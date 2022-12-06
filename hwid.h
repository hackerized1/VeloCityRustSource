#pragma once
#include <windows.h>
#include <intrin.h>
#include <iphlpapi.h>
#include <sstream>
#include "xor.h"

namespace hwid
{
	uint16_t hashMacAddress(PIP_ADAPTER_INFO info)
	{
		VM_DOLPHIN_BLACK_START
		__nop(); //NOP 
		uint16_t hash = 0; __nop(); //NOP 
		for (uint32_t i = 0; i < info->AddressLength; i++)
		{
			hash += (info->Address[i] << ((i & 1) * 8));
		}
		__nop(); //NOP 
		return hash;

		VM_DOLPHIN_BLACK_END
	}

	uint16_t getVolumeHash()
	{
		VM_DOLPHIN_BLACK_START

		DWORD serialNum = 0; __nop(); //NOP 
		GetVolumeInformation(xorstr("c:\\"), nullptr, 0, &serialNum, nullptr, nullptr, nullptr, 0); __nop(); //NOP 
		uint16_t hash = static_cast<uint16_t>((serialNum + (serialNum >> 16)) & 0xFFFF); __nop(); //NOP 
		return hash;

		VM_DOLPHIN_BLACK_END
	}

	uint16_t getCpuHash()
	{
		VM_DOLPHIN_BLACK_START

		int cpuinfo[4] = { 0, 0, 0, 0 }; __nop(); //NOP 
		__cpuid(cpuinfo, 0); __nop(); //NOP 
		uint16_t hash = 0; __nop(); //NOP 
		uint16_t* ptr = (uint16_t*)(&cpuinfo[0]); __nop(); //NOP 
		for (uint32_t i = 0; i < 8; i++)
			hash += ptr[i];

		return hash;

		VM_DOLPHIN_BLACK_END
	}

	const char* getMachineName()
	{
		VM_DOLPHIN_BLACK_START

		static char computerName[1024]; __nop(); //NOP 
		DWORD size = 1024; __nop(); //NOP 
		GetComputerName(computerName, &size);

		return &(computerName[0]);

		VM_DOLPHIN_BLACK_END
	}


	std::string generateHash(const std::string& bytes)
	{
		VM_DOLPHIN_BLACK_START

		static char chars[] = "0123456789ABCDEF";
		std::stringstream stream;

		auto size = bytes.size();
		for (unsigned long i = 0; i < size; ++i)
		{
			unsigned char ch = ~static_cast<unsigned char>(static_cast<unsigned short>(bytes[i]) +
				static_cast<unsigned short>(bytes[(i + 1) % size]) +
				static_cast<unsigned short>(bytes[(i + 2) % size]) +
				static_cast<unsigned short>(bytes[(i + 3) % size])) * (i + 1);

			stream << chars[(ch >> 4) & 0x0F] << chars[ch & 0x0F];
		}

		return stream.str();

		VM_DOLPHIN_BLACK_END
	}

	std::string get()
	{
		VM_DOLPHIN_BLACK_START

		static const unsigned long TargetLength = 32;

		__nop(); //NOP 
		std::stringstream stream;

		stream << getMachineName(); __nop(); //NOP 
		stream << getCpuHash();
		stream << getVolumeHash(); __nop(); //NOP 

		auto string = stream.str();

		while (string.size() < TargetLength)
		{
			string = string + string;
		}

		if (string.size() > TargetLength)
		{
			__nop(); //NOP 
			string = string.substr(0, TargetLength);
		}

		return generateHash(string);

		VM_DOLPHIN_BLACK_END
	}
}