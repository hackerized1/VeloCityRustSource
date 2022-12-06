#pragma once
#include "structures.h"
#include "driver/include.h"
#include "sdk.h"

namespace player
{
    bool checkFlag(const structures::BasePlayer_t& plr, int flag)
    {
        return plr.playerFlags & (flag << 0);
    }

    void removeFlagModel(const structures::BasePlayer_t& plr, int flag)
    {
        int flags = driver::rpm<int>(plr.modelState + 0x24);
        driver::wpm<int>(flags &= ~flag, plr.modelState + 0x24);
    }

    bool checkFlagModel(const structures::BasePlayer_t& plr, int flag)
    {
        return driver::rpm<int>(plr.modelState + 0x24) & flag;
    }

    bool isVisible(const structures::BasePlayer_t& plr)
    {
        return driver::rpm<bool>(plr.playerModel + 0x248);
    }

    bool getBelt(const structures::BasePlayer_t& plr, structures::BeltItems_t& out)
    {
        if (!plr.inventory) return 0;


        uintptr_t containerBelt = driver::rpm<uintptr_t>(plr.inventory + 0x28); //public ItemContainer containerBelt; // 0x28
        if (!containerBelt) return 0;

        uintptr_t contents = driver::rpm<uintptr_t>(containerBelt + 0x38); // public List<Item> contents; // 0x38
        if (!contents) return 0;


        uintptr_t contentsList = driver::rpm<uintptr_t>(contents + 0x10);
        if (!contentsList) return 0;


        for (int i = 0; i < 6; ++i)
        {
            uintptr_t addy = driver::rpm<uintptr_t>(contentsList + 0x20 + (i * 0x8));
            if (!addy)  continue;

            out.slot[i] = driver::rpm<structures::Item_t>(addy);

        }

        return true;
    }

    structures::str_w getName(const structures::BasePlayer_t& plr)
    {

        //uintptr_t displayName = driver::rpm<uintptr_t>(plr.address + 0x658);
        //if (!displayName) return {};

        return driver::rpm<structures::str_w>(plr._displayName);
    }

    structures::Item_t getActiveItem(const structures::BasePlayer_t& plr)
    {
        //std::cout << "weapon \n" << std::endl;

        if (plr.clActiveItem)
        {
            structures::BeltItems_t items = { 0 };
            if (getBelt(plr, items))
            {
                for (int i = 0; i < 6; ++i)
                {
                    if (!items.slot[i].uid) continue;

                    if (plr.clActiveItem == items.slot[i].uid) return items.slot[i];
                }

            }
        }

        return { 0 };

    }

    math::vector3 getBasePosition(const structures::BasePlayer_t& plr)
    {
        if (plr.playerModel)
        {
            structures::PlayerModel_t playerModel = driver::rpm<structures::PlayerModel_t>(plr.playerModel);
            return playerModel.position;

        }

        return { 0, 0,0 };
    }

    math::vector3 getBonePosition(const structures::BasePlayer_t& target, int index)
    {
        if (!target.model) return { 0,0,0 };

        uintptr_t boneTransforms = driver::rpm<uintptr_t>(target.model + 0x48);
        if (!boneTransforms) return { 0,0,0 };

        uintptr_t boneVal = driver::rpm<uintptr_t>(boneTransforms + (0x20 + index * 0x8));
        if (!boneVal) return { 0,0,0 };

        uintptr_t bone = driver::rpm<uintptr_t>(boneVal + 0x10);
        if (!bone) return { 0,0,0 };

        return sdk::getTransformPos(bone);
    }

    bool isInSameTeam(const structures::BasePlayer_t& plr1, const structures::BasePlayer_t& plr2)
    {
        if (!plr1.currentTeam || !plr2.currentTeam) return 0;

        if (plr1.currentTeam == plr2.currentTeam) return 1;

        return 0;
    }
}