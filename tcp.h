#pragma once

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "Ws2_32.lib")
#include <windows.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <vector>

#include "log.h"
#include "headers.h"
#include "aes.h"
#include "rsa.h"
#include "xor.h"
#include "themida.h"

#define PORT 6543

#pragma warning(disable : 4996)

//tcp client
namespace tcp
{
	struct packet_metadata_t
	{
		unsigned int size;
		unsigned char iv[16];
	};

	//socket
	SOCKET socket;

	//aes key
	unsigned char key[32] = { 0 };

	bool startup()
	{
		VM_DOLPHIN_BLACK_START
			WSADATA wsadata;
		int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
		if (result > 0) {
			Log(c::fail, "[error] WSAStartup failed: %i\n", WSAGetLastError());
			return false;
		}

		socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socket == INVALID_SOCKET) {
			Log(c::fail, "[error] socket failed: %i\n", WSAGetLastError());
			return false;
		}

		struct sockaddr_in serv_addr = { 0 };
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		//"65.21.52.135"
		//"127.0.0.1"
		if (inet_pton(AF_INET, xorstr("65.21.52.135"), &serv_addr.sin_addr) <= 0) {
			Log(c::fail, "[error] inet_pton failed: %i\n", WSAGetLastError());
			return false;
		}

		if (connect(socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
			Log(c::fail, "[error] connect failed: %i\n", WSAGetLastError());
			return false;
		}

		return true;
		VM_DOLPHIN_BLACK_END
	}

	namespace L1
	{
		unsigned int max_allowed_packet_size = 0xFFFFFFFFFF;

		bool recv_metadata(packet_metadata_t& out)
		{
				//recv metadata
				packet_metadata_t metadata = { 0 };
			int bytes = recv(socket, (char*)&metadata, sizeof metadata, MSG_WAITALL);
			if (bytes < 1)
				return false;

			out = metadata;

			//check if the metadata size is not bigger than allowed
			if (metadata.size > max_allowed_packet_size)
			{
				Log(c::sus, "%x sent more than allowed amount [%x/%x]\n", socket, metadata.size, max_allowed_packet_size);
				return false;
			}

			return true;
		}

		bool send_metadata(const packet_metadata_t& metadata)
		{
				int sent = send(socket, (const char*)&metadata, sizeof(metadata), 0);
			return sent == sizeof(metadata);
		}
	}

	namespace L2
	{
		template <class T>
		bool send_packet(T& packet)
		{
				return send_raw(socket, (void*)&packet, sizeof T);
		}

		bool recv_raw(std::vector<unsigned char>& out)
		{
				//get metadata
				packet_metadata_t metadata = { 0 };
			if (!L1::recv_metadata(metadata)) return false;

			//allocate enough memory
			std::vector<unsigned char> data;
			data.resize(metadata.size);

			//recv data
			int bytes = recv(socket, (char*)data.data(), metadata.size, MSG_WAITALL);
			if (bytes < 1) return false;

			out = data;
			return true;
		}

		bool send_raw(std::vector<unsigned char> data)
		{
				//set up metadata
				packet_metadata_t metadata = { 0 };
			metadata.size = data.size();

			//send metadata
			if (!L1::send_metadata(metadata)) return false;

			//send encrypted data
			int sent = send(socket, (const char*)data.data(), data.size(), 0);
			if (sent < 1) return false;

			return true;
		}

		bool send_raw(void* data, unsigned int size)
		{
				//set up metadata
				packet_metadata_t metadata = { 0 };
			metadata.size = size;

			//send metadata
			if (!L1::send_metadata(metadata)) return false;

			//send encrypted data
			int sent = send(socket, (const char*)data, size, 0);
			if (sent < 1) return false;

			return true;
		}

		bool recv_enc_raw(std::vector<unsigned char>& out)
		{
				//get metadata
				packet_metadata_t metadata = { 0 };
			if (!L1::recv_metadata(metadata)) return false;

			//allocate enough memory
			std::vector<unsigned char> data;
			data.resize(metadata.size);

			//recv data
			int bytes = recv(socket, (char*)data.data(), metadata.size, MSG_WAITALL);
			if (bytes < 1) return false;

			unsigned char iv[16];
			memcpy(iv, metadata.iv, 16);

			out.resize(metadata.size + 16);
			auto sz = encryption::aes::encrypt_decrypt(key, iv, (unsigned char*)data.data(), (unsigned char*)out.data(), metadata.size, AES_DECRYPT);
			out.resize(sz);

			return sz > 0;
		}

		template <class T>
		bool recv_enc_packet(T& packet_out)
		{
				//get metadata
				packet_metadata_t metadata = { 0 };
			if (!L1::recv_metadata(metadata)) return false;

			//allocate enough memory
			std::vector<unsigned char> data;
			std::vector<unsigned char> data_out;
			data.resize(metadata.size);
			data_out.resize(metadata.size + 16);

			//recv data
			int bytes = recv(socket, (char*)data.data(), metadata.size, MSG_WAITALL);
			if (bytes < 1)
				return false;

			unsigned char iv[16];
			memcpy(iv, metadata.iv, 16);

			auto sz = encryption::aes::encrypt_decrypt(key, iv, (unsigned char*)data.data(), (unsigned char*)data_out.data(), metadata.size, AES_DECRYPT);
			data_out.resize(sz);

			if (sizeof(T) == sz)
			{
				memcpy(&packet_out, data_out.data(), sizeof T);
			}
			else {
				return false;
			}

			return true;
		}

		bool send_enc_raw(std::vector<unsigned char>& data)
		{
				//generate iv
				unsigned char iv[16] = { 0 };
			RAND_bytes(iv, sizeof iv);

			//encryption buffer
			std::vector<unsigned char> out;
			out.resize(data.size() + 16);

			//encrypt the data
			auto sz = encryption::aes::encrypt_decrypt(key, iv, data.data(), out.data(), data.size(), AES_ENCRYPT);
			out.resize(sz);

			//set up metadata
			packet_metadata_t metadata = { 0 };
			memcpy(metadata.iv, iv, sizeof(iv));
			metadata.size = sz;

			//send metadata
			if (!L1::send_metadata(metadata)) return false;

			//send encrypted data
			int sent = send(socket, (const char*)out.data(), out.size(), 0);
			if (sent < 1) return false;

			return true;
		}

		bool send_enc_raw(void* data, unsigned int size)
		{
				//generate iv
				unsigned char iv[16] = { 0 };
			RAND_bytes(iv, sizeof iv);

			//encryption buffer
			std::vector<unsigned char> out;
			out.resize(size + 16);

			//encrypt the data
			auto sz = encryption::aes::encrypt_decrypt(key, iv, (unsigned char*)data, out.data(), size, AES_ENCRYPT);
			out.resize(sz);

			//set up metadata
			packet_metadata_t metadata = { 0 };
			memcpy(metadata.iv, iv, sizeof(iv));
			metadata.size = sz;

			//send metadata
			if (!L1::send_metadata(metadata)) return false;

			//send encrypted data
			int sent = send(socket, (const char*)out.data(), out.size(), 0);
			if (sent < 1) return false;

			return true;
		}

		template <class T>
		bool send_enc_packet(T& packet)
		{
				//generate iv
				unsigned char iv[16] = { 0 };
			RAND_bytes(iv, sizeof iv);

			//encryption buffer
			std::vector<unsigned char> out;
			out.resize(sizeof T + 16);

			//encrypt the data
			auto sz = encryption::aes::encrypt_decrypt(key, iv, (unsigned char*)&packet, out.data(), sizeof(T), AES_ENCRYPT);
			out.resize(sz);

			//set up metadata
			packet_metadata_t metadata = { 0 };
			memcpy(metadata.iv, iv, sizeof(iv));
			metadata.size = sz;

			//send metadata
			if (!L1::send_metadata(metadata)) return false;

			//send packet
			int sent = send(socket, (const char*)out.data(), out.size(), 0);
			if (sent < 1) return false;

			return true;
		}
	}

	namespace w
	{
		bool get_resource(std::string name, std::vector<unsigned char>& out)
		{
			std::vector<unsigned char> data;
			auto packet = make<p::get_resource>(packet_ids::get_resource);
			strcpy_s(packet.name, name.c_str());

			if (!L2::send_enc_packet(packet)) return false;

			p::feedback_t feedback = { 0 };
			L2::recv_enc_packet<p::feedback_t>(feedback);

			if (feedback.failed == 1) return false;

			//download the file
			bool ret = L2::recv_enc_raw(out);

			//tell server that we finished downloading
			auto done = make<p::packet_base_t>(packet_ids::done_downloading);
			tcp::L2::send_enc_packet(done);

			return ret;
		}

		//bool download_module(std::string shortname, std::vector<unsigned char>& out)
		//{
		//	VM_DOLPHIN_BLACK_START
		//
		//		auto filereq = make<p::file_request_t>(packet_ids::filerequest);
		//	strcpy_s(filereq.name, shortname.c_str());
		//
		//	if (!tcp::L2::send_enc_packet(filereq)) return false;
		//	p::feedback_t feedback = { 0 };
		//	if (!tcp::L2::recv_enc_packet(feedback)) return false;
		//	if (feedback.failed == true) return false;
		//
		//	//download the file
		//	return L2::recv_enc_raw(out);
		//
		//	VM_DOLPHIN_BLACK_END
		//}

	}

	namespace handshake
	{
		bool perform()
		{

			//generate rsa key pair
			RSA* rsa_key = encryption::rsa::generate(bits_::b4096);

			VM_DOLPHIN_BLACK_START

			//send public rsa key
			auto keys = make<p::keys_t>(packet_ids::keys);
			auto public_key = encryption::rsa::get_public_key_pem(rsa_key);

			strcpy_s(keys.key, public_key);
			send(socket, (const char*)&keys, sizeof keys, 0);

			//recv encrypted aes keys and decrypt them
			std::vector<unsigned char> aes_key_enc;
			if (!tcp::L2::recv_raw(aes_key_enc))
			{
				//failiure
				RSA_free(rsa_key);
				free(public_key);
				return false;
			}

			//decrypt
			std::vector<unsigned char> aes_key;
			aes_key = encryption::rsa::wrapper_decrypt(rsa_key, aes_key_enc.data(), aes_key_enc.size());

			memcpy(key, (void*)aes_key.data(), 32);
			RSA_free(rsa_key);
			free(public_key);

			return true;
			VM_DOLPHIN_BLACK_END
		}
	}
}