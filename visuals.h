#pragma once
#include "player.h"
#include "camera.h"
#include "item.h"
#include "weapon.h"

enum chams
{
    translucent,
    zwhite,
    zblack,
    pink
};

chams chams_selection;

namespace visuals
{
    uintptr_t get_material()
    {
        if (dome::TOD_Components)
        {
            uintptr_t TOD_Scattering = driver::rpm<uintptr_t>(dome::TOD_Components + 0x1A0); //scatteringMaterial
            if (!TOD_Scattering) return -1;

            switch (chams_selection)
            {
            case translucent:
                return driver::rpm<uintptr_t>(dome::TOD_Components + 0x150);
                break;
            case zwhite:
                return driver::rpm<uintptr_t>(TOD_Scattering + 0x78);
                break;
            case zblack:
                return driver::rpm<uintptr_t>(TOD_Scattering + 0x88);
                break;
            case pink:
                return 0;
                break;
            default:
                return -1;
                break;
            }

            //return driver::rpm<uintptr_t>(dome::TOD_Components + 0x150); //translucent
            //return driver::rpm<uintptr_t>(TOD_Scattering + 0x78); //zchams white
            //return driver::rpm<uintptr_t>(TOD_Scattering + 0x88); //zchams black
            //return 0; //pink
        }
    }

    void run_chams(const structures::BasePlayer_t& entity)
    {
        if (!entity.playerModel) return;

        structures::PlayerModel_t playerModel = driver::rpm<structures::PlayerModel_t>(entity.playerModel);
        if (!playerModel._multiMesh) return;

        uintptr_t SkinCollection = driver::rpm<uintptr_t>(playerModel._multiMesh + 0x68);
        if (!SkinCollection) return;

        uintptr_t mat = get_material();

        uintptr_t skins = driver::rpm<uintptr_t>(SkinCollection + 0x18);
        if (!skins) return;

        for (int i = 0; i <= driver::rpm<int>(skins + 0x18); i++)
        {
            uintptr_t cur_skin = driver::rpm<uintptr_t>(skins + 0x20 + (i * 0x8));
            if (!cur_skin) continue;


            uintptr_t ptr1 = driver::rpm<uintptr_t>(cur_skin + 0x68);
            if (ptr1 != mat)
            {
                driver::wpm<uintptr_t>(mat, cur_skin + 0x68);
            }

            uintptr_t ptr2 = driver::rpm<uintptr_t>(cur_skin + 0x70);
            if (ptr1 != mat)
            {
                driver::wpm<uintptr_t>(mat, cur_skin + 0x70);
            }

            uintptr_t ptr3 = driver::rpm<uintptr_t>(cur_skin + 0x78);
            if (ptr1 != mat)
            {
                driver::wpm<uintptr_t>(mat, cur_skin + 0x78);
            }
        }

        driver::wpm<float>(400.f, globals::classes::culling + 0x4);

    }

}