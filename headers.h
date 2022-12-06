#pragma once
#include <vector>

enum packet_ids
{
	disconnect = 0,
	//reserved
	feedback,
	login,
	filerequest,
	subtime,
	keys,
	requesttoken,
	redeemtoken,
	stayalive,
	set_product,
	get_configs,
	set_config,
	get_resource,
	done_downloading,
	steamid,

	//admin commands
	requestdb = 50,
	adduser,
	updatecache,
	updatefile,
};
#define DATA_MESSAGE 64
#define DATA_SIZE 64
#define DATA_USERNAME 16
#define DATA_PASSWORD 32
#define DATA_HWID 64
#define DATA_SUB_NAME 64
#define DATA_SUB_SHORT_NAME 16
#define DATA_SUB_DESC 64
#define DATA_MAX_SUBS 8

#define DATA_RES_NAME_SIZE 32

#define DATA_CFG_NAME_SIZE 16
#define DATA_CFG_LINE_SIZE 64
#define DATA_CFG_ARRAY_SIZE 100

#define DATA_RSA_KEY 1024

//extra structs that packets will use
//define them outside the namespace
struct sub_t
{
	bool frozen;
	int global_status;
	long long time_left_seconds;
	long long last_usage_epoch;
	char name[DATA_SUB_SHORT_NAME];
	char display_name[DATA_SUB_NAME];
};

struct line_t
{
	char buffer[DATA_CFG_LINE_SIZE] = { 0 };
};

namespace p
{
	struct packet_base_t
	{
		unsigned int id;
	};

	struct steamid : packet_base_t
	{
		uint64_t id;
	};

	struct get_resource : packet_base_t
	{
		char name[DATA_RES_NAME_SIZE];
	};

	struct config_t : packet_base_t
	{
		char name[DATA_CFG_NAME_SIZE];
		line_t lines[DATA_CFG_ARRAY_SIZE];
	};

	struct set_product_t : packet_base_t
	{
		char short_name[DATA_SUB_SHORT_NAME];
		long long boot_id;
	};

	struct keys_t : packet_base_t
	{
		char key[DATA_RSA_KEY];
	};

	struct feedback_t : packet_base_t
	{
		char message[DATA_MESSAGE];
		char data[DATA_SIZE];
		bool failed;
	};

	struct login_t : packet_base_t
	{
		char username[DATA_USERNAME];
		char password[DATA_PASSWORD];
		char hwid[DATA_HWID];
	};

	struct file_request_t : packet_base_t
	{
		char name[16];
		char data[DATA_SIZE];
	};

	//depricated
	//struct file_metadata_t : packet_base_t
	//{
	//	char filename[16];
	//	char extension[8];
	//	unsigned int bytes;
	//};

	struct sub_time_t : packet_base_t
	{
		sub_t subscriptions[DATA_MAX_SUBS];
	};

	struct request_token_t : packet_base_t
	{
		int item_name_hash;
		char hwid[DATA_HWID];
	};

	struct redeem_token_t : packet_base_t
	{
		int item_name_hash;
		char hwid[DATA_HWID];
		char code[DATA_SIZE];
	};
}

template <typename T>
T format(std::vector<unsigned char>& buffer)
{
	T temp;
	int size = sizeof T;
	memset(&temp, 0, size);

	if (buffer.size() < size)
	{
		//sent data is smaller
		memcpy(&temp, buffer.data(), buffer.size());
	}
	else if (buffer.size() > size)
	{
		//sent data is bigger
		memcpy(&temp, buffer.data(), size);
	}
	else if (buffer.size() == size)
	{
		memcpy(&temp, buffer.data(), size);
	}

	return temp;
}

template <typename T>
T make(int packet_id)
{
	T packet;
	memset((void*)&packet, 0, sizeof T);
	p::packet_base_t* ppacket = &packet;
	ppacket->id = packet_id;
	return packet;
}
