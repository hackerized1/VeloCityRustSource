#pragma once
#include "globals.h"
#include "driver/include.h"
#include "config.h"
#include "sdk.h"
#include "defs.h"
#include "player.h"

namespace misc
{
    void spider()
    {
        if (!globals::objects::localPlayer.movement) return;

        driver::wpm<math::vector3>(math::vector3(0, 999999999999, 0), globals::objects::localPlayer.movement + 0xC8);
    }

    void forceSprint()
    {
        if (!globals::objects::localPlayer.movement) return;

        driver::wpm<bool>(1, globals::objects::localPlayer.movement + 0x134);
    }

    void alwaysDay()
    {
        driver::wpm<float>((int)ui::sliderf_misc_time_value->get(), globals::classes::admin);
    }

    void noUnderwaterEffect()
    {
        driver::wpm<bool>(!config::misc::noUnderwaterEffect, globals::classes::admin + 0x5);
        driver::wpm<float>(config::misc::noUnderwaterEffect ? 0.0001 : 1, globals::classes::admin + 0x8); //underwater_scatter
    }

    void debugCameraBypass()
    {
        driver::wpm <float>(1.f, globals::classes::client + 0x2C);
        driver::wpm <float>(1.f, globals::classes::client + 0x20);
    }

    void noHeadCollision(bool state)
    {
        if (!globals::objects::localPlayer.movement) return;

        if (state)
        {
            driver::wpm<float>(0.f, globals::objects::localPlayer.movement + 0x68);
            driver::wpm<float>(0.f, globals::objects::localPlayer.movement + 0x6C);
        }
        else
        {
            driver::wpm<float>(1.8f, globals::objects::localPlayer.movement + 0x68);
            driver::wpm<float>(0.9f, globals::objects::localPlayer.movement + 0x6C);
        }
    }

    void noclip(bool state, bool prevent_kick)
    {
        static math::vector3 start_pos;
        if (!globals::objects::localPlayer.movement) return;

        if (state)
        {
            math::vector3 cur_pos = player::getBasePosition(globals::objects::localPlayer);
            if (start_pos.empty()) start_pos = cur_pos;

            math::vector3 targetMovement;

            math::vector4 rotation = driver::rpm<math::vector4>(globals::objects::localPlayer.eyes + 0x44);

            if (GetAsyncKeyState('W'))
            {
                targetMovement = targetMovement + sdk::multiplyQuad(rotation, math::vector3(0, 0, 1));
            }

            if (GetAsyncKeyState('S'))
            {
                targetMovement = targetMovement - sdk::multiplyQuad(rotation, math::vector3(0, 0, 1));
            }

            if (GetAsyncKeyState('A'))
            {
                targetMovement = targetMovement - sdk::multiplyQuad(rotation, math::vector3(1, 0, 0));
            }

            if (GetAsyncKeyState('D'))
            {
                targetMovement = targetMovement + sdk::multiplyQuad(rotation, math::vector3(1, 0, 0));
            }
            if (GetAsyncKeyState(VK_SPACE))
            {
                targetMovement.y += 1.f;
            }

            float speed = 5.f;

            if (GetAsyncKeyState(VK_CONTROL))
            {
                speed = 2.5f;
            }

            if (GetAsyncKeyState(VK_SHIFT))
            {
                speed = 10.f;
            }

            targetMovement = targetMovement * speed;

            if (!targetMovement.empty())
            {
                if (prevent_kick)
                {
                    if ((cur_pos.y - start_pos.y) > 4.f)
                        targetMovement.y = 0;

                    if ((cur_pos.z - start_pos.z) > 1.f || (cur_pos.x - start_pos.x) > 1.f)
                    {
                        targetMovement.z = 0;
                        targetMovement.x = 0;
                    }
                }

                driver::wpm<math::vector3>(targetMovement, globals::objects::localPlayer.movement + 0x34);

            }


        }
        else
        {
            driver::wpm<float>(2.5, globals::objects::localPlayer.movement + 0x84);

            driver::wpm<bool>(0, globals::objects::localPlayer.movement + 0x14C);
            driver::wpm<bool>(0, globals::objects::localPlayer.movement + 0x14D);

            driver::wpm<float>(0.9f, globals::objects::localPlayer.movement + 0x6C);


            driver::wpm<float>(0, globals::objects::localPlayer.address + offsetof(structures::BasePlayer_t, clothingMoveSpeedReduction));

            start_pos = {};
        }
    }

    void changeFOV(float value)
    {
        driver::wpm<float>(value, globals::classes::graphics + 0x18);
    }

    void admin()
    {
        if (!globals::objects::localPlayer.playerFlags) return;

        if (player::checkFlag(globals::objects::localPlayer, FL_SLEEPING)) return;

        driver::wpm<int>((globals::objects::localPlayer.playerFlags |= 4), globals::objects::localPlayer.address + offsetof(structures::BasePlayer_t, playerFlags));
    }

    void boostVoicechatVolume(float value)
    {
        driver::wpm<float>(value, globals::classes::audio + 0x10);
    }

    void fastUpdateCulling()
    {
        driver::wpm<float>(99999, globals::classes::culling);
    }

    void disallow_server_commands()
    {
        uintptr_t ConsoleSystemCommands = driver::rpm<uintptr_t>(globals::classes::index + 0x10);
        if (!ConsoleSystemCommands) return;

        for (int x = 0; x < 500; x++)
        {
            uintptr_t ConsoleSystemCommand = driver::rpm<uintptr_t>(ConsoleSystemCommands + 0x20 + (x * 0x8));
            if (!ConsoleSystemCommand) continue;

            uintptr_t Name = driver::rpm<uintptr_t>(ConsoleSystemCommand + 0x10);
            if (!Name) continue;

            wchar_t command_name[36] = { '\0' };
            driver::rpm_raw(Name + 0x14, (uintptr_t)&command_name, sizeof(command_name));

            if (!wcscmp(command_name, L"noclip"))
            {
                driver::wpm<bool>(0, ConsoleSystemCommand + 0x57);
            }
        }
    }

    void sidePeek(bool b)
    {
        if (b)
        {
            math::vector3 direction;
            if(input::key_down(ui::key_misc_side_peek_left->get()))
            {
                direction.x = -1.5f;
            }
            else if (input::key_down(ui::key_misc_side_peek_forward->get()))
            {
                direction.z = 1.5f;
            }
            else if (input::key_down(ui::key_misc_side_peek_right->get()))
            {
                direction.x = 1.5f;
            }

            if (!direction.empty())
            {
                driver::wpm<float>(0.99f, globals::objects::localPlayer.address + offsetof(structures::BasePlayer_t, clientTickInterval));
                driver::wpm<math::vector3>(direction, globals::objects::localPlayer.eyes + 0x38);
            }
        }
        else
        {
            driver::wpm<float>(0.2f, globals::objects::localPlayer.address + offsetof(structures::BasePlayer_t, clientTickInterval));
        }
    }

    void cullingdebug()
    {
        uintptr_t instance = driver::rpm<uintptr_t>(globals::classes::occlusionCulling + 0x80);
        if (!instance) return;

        uintptr_t debugSettings = driver::rpm<uintptr_t>(instance + 0x18);
        if (!debugSettings) return;

        driver::wpm<int>(131072, debugSettings + 0x20);
        driver::wpm<int>(1, globals::classes::occlusionCulling + 0x94);
    }
}