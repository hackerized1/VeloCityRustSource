#pragma once
#include "../utils.h"
#include "../xor.h"
#include "def.h"

#ifdef _DEBUG
#include "dbg.h"
#else
#include "main.h"
#endif

namespace driver
{

	uint64_t rpm_chain(uint64_t start, std::vector<uint64_t> offsets)
	{
		uint64_t ptr = start;

		for (int i = 0; i < offsets.size(); i++)
		{
			ptr = driver::rpm<uint64_t>(ptr + offsets[i]);
			if (ptr == 0) return 0;
		}

		return ptr;
	}

	template <typename T>
	T rpm_wait(uintptr_t addy, int delay = 3000)
	{
		while (!rpm<T>(addy)) Sleep(delay);

		return rpm<T>(addy);
	}

}