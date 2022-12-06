#pragma once
#include "driver/include.h"
#include "structures.h"
#include "xor.h"
#include "fnv.h"

namespace item
{
    structures::str_w getName(const structures::Item_t& item)
    {
        if (item.uid && item.info)
        {
            uint64_t displayname = driver::rpm<uint64_t>(item.info + 0x28);
            if (displayname)
            {
                uint64_t english = driver::rpm<uint64_t>(displayname + 0x18);
                if (english)
                {
                    return driver::rpm<structures::str_w>(english);
                }

            }
        }

        return { 0 };
    }


    int getCategory(const structures::Item_t& item)
    {
        return driver::rpm<int>(item.info + 0x40);
    }

    structures::str getShortname(const structures::Item_t& item)
    {
        if (item.uid && item.info)
        {
            uint64_t shortname = driver::rpm<uint64_t>(item.info + 0x20);
            if (shortname)
            {
                return driver::rpm<structures::str>(shortname);

            }
        }

        return { 0 };
    }

    bool isWeapon(uint32_t hash)
    {
        if (hash == fnv::hash(xorstr("rifle.ak")) ||
            hash == fnv::hash(xorstr("rifle.ak.ice")) ||
            hash == fnv::hash(xorstr("lmg.m249")) ||
            hash == fnv::hash(xorstr("rifle.semiauto")) ||
            hash == fnv::hash(xorstr("pistol.revolver")) ||
            hash == fnv::hash(xorstr("pistol.python")) ||
            hash == fnv::hash(xorstr("rifle.m39")) ||
            hash == fnv::hash(xorstr("rifle.lr300")) ||
            hash == fnv::hash(xorstr("smg.thompson")) ||
            hash == fnv::hash(xorstr("pistol.semiauto")) ||
            hash == fnv::hash(xorstr("smg.mp5")) ||
            hash == fnv::hash(xorstr("smg.2")) ||
            hash == fnv::hash(xorstr("rifle.l96")) ||
            hash == fnv::hash(xorstr("rifle.bolt")) ||
            hash == fnv::hash(xorstr("bow.hunting")) ||
            hash == fnv::hash(xorstr("pistol.m92")))
        {
            return 1;
        }
        return 0;
    }
}
