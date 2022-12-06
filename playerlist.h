#pragma once
#include "driver/include.h"
#include "globals.h"

namespace playerlist
{
	uintptr_t address;
	uintptr_t size;
	uintptr_t array;

	std::vector<structures::BasePlayer_t> list;

	bool init()
	{
		MutateStart();

		uintptr_t p = driver::rpm<uintptr_t>(globals::classes::basePlayer + 0x10);
		if (!p) return 0;


		array = driver::rpm<uintptr_t>(p + 0x28);
		if (!array) return 0;

		return 1;

		MutateEnd();
	}

	structures::BasePlayer_t get_entry(int index)
	{
		uintptr_t memberBase = driver::rpm<uintptr_t>(address + 0x20 + (index * 0x8));
		if (!memberBase) return { 0 };

		structures::BasePlayer_t ret;
		driver::rpm_raw(memberBase, (uintptr_t)&ret, sizeof(structures::BasePlayer_t) - sizeof(uintptr_t) /*  uintptr_t address */);

		ret.address = memberBase;

		return ret;
	}

	bool get()
	{
		list.clear();

		size = driver::rpm<int>(array + 0x10);
		address = driver::rpm<uintptr_t>(array + 0x18);

		if (!address || !size) return 0;

		list.resize(size);
		for (int i = 0; i < playerlist::size; i++)
		{
			list[i] = get_entry(i);
		}

		return 1;
	}

	structures::BasePlayer_t& getMember(int index)
	{
		return list[index];
	}

	structures::BasePlayer_t find(uintptr_t address)
	{
		for (const auto& p : list)
		{
			if (p.address == address)
				return p;
		}

		return { 0 };
	}
}