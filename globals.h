#pragma once
#include <cstdint>
#include "structures.h"

namespace globals
{
    bool init = 0;
    bool initialized_time;

    uintptr_t gameAssemblyMod = 0;
    uintptr_t unityPlayerMod = 0;

    math::vector2 gameSize = { 0, 0 };

    namespace objects
    {
        structures::BasePlayer_t localPlayer = { 0 };
        structures::Item_t localActiveWep = { 0 };
        uintptr_t camera = 0;
    }

    namespace classes
    {
        uintptr_t cameraManager = 0;
        uintptr_t baseEntity = 0;
        uintptr_t basePlayer = 0;
        uintptr_t admin = 0;
        uintptr_t client = 0;
        uintptr_t graphics = 0;
        uintptr_t audio = 0;
        uintptr_t culling = 0;
        uintptr_t index = 0;
        uintptr_t occlusionCulling = 0;
        uintptr_t TOD_Sky = 0;
        uintptr_t MainCamera = 0;
    }
}