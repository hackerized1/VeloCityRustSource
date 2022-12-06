#pragma once
#include "driver/include.h"
#include "globals.h"
#include "config.h"

namespace dome
{
    uintptr_t TOD_DayParameters;
    uintptr_t TOD_AmbientParameters;
    uintptr_t TOD_StarParameters;
    uintptr_t TOD_Sky;
    uintptr_t TOD_NightParameters;
    uintptr_t TOD_MoonParameters;
    uintptr_t TOD_LightParameters;
    uintptr_t TOD_ReflectionParameters;
    uintptr_t TOD_Components;

    bool frozen_ambient = false;

    bool fetch()
    {
        //uint64_t taggedObject = driver::rpm<uint64_t>(globals::classes::gameObjectManager + 0x8, 2);
        //if (!taggedObject) return 0;
        //taggedObject = driver::rpm<uint64_t>(taggedObject + 0x8, 2);
        //if (!taggedObject) return 0;
        //uintptr_t dome = driver::rpm<uint64_t>(taggedObject + 0x10, 2);
        //if (!dome) return 0;

        //uint64_t p = driver::rpm<uint64_t>(dome + 0x30, 2);
        //if (!p) return 0;
        //uint64_t p1 = driver::rpm<uint64_t>(p + 0x18, 2);
        //if (!p1) return 0;
        //uint64_t addy = driver::rpm<uint64_t>(p1 + 0x28, 2);
        //if (!addy) return 0;


        uintptr_t instances = driver::rpm<uint64_t>(globals::classes::TOD_Sky);
        if (!instances) return 0;

        uint32_t size = driver::rpm<uint32_t>(instances + 0x18);
        for (auto i = 0; i < size; i++)
        {
            const auto list = driver::rpm<uintptr_t>((uintptr_t)instances + 0x10);
            if (!list) return 0;

            const auto internal_list = list + 0x20;
            uintptr_t buf = driver::rpm<uint64_t>(internal_list + i * sizeof(uint64_t));


            TOD_DayParameters = driver::rpm<uint64_t>(buf + 0x50);
            if (!TOD_DayParameters) continue;

            TOD_AmbientParameters = driver::rpm<uint64_t>(buf + 0x90);
            if (!TOD_AmbientParameters) continue;

            TOD_MoonParameters = driver::rpm<uint64_t>(buf + 0x68);
            if (!TOD_MoonParameters) continue;

            TOD_NightParameters = driver::rpm<uint64_t>(buf + 0x58);
            if (!TOD_NightParameters) continue;

            TOD_StarParameters = driver::rpm<uint64_t>(buf + 0x70);
            if (!TOD_StarParameters) continue;

            TOD_Components = driver::rpm<uint64_t>(buf + 0xA8);
            if (!TOD_Components) continue;

            return true;

        }

        return false;
    }

    void bigStars()
    {
        driver::wpm<float>(5.f, TOD_StarParameters + 0x10);
        driver::wpm<float>(5, TOD_StarParameters + 0x14);
        driver::wpm<int>(1, TOD_StarParameters + 0x18);
    }

    void bigMoon()
    {
        driver::wpm<float>(12.f, TOD_MoonParameters + 0x10);
    }

    void ambientMultiplier()
    {
        float updateInterval = 0.15f;

        if (!frozen_ambient)
        {
            driver::wpm<float>(updateInterval, TOD_AmbientParameters + 0x18);
        }

        if (ui::chk_misc_nightmode->get()) driver::wpm<float>(3.f, TOD_NightParameters + 0x50);
        if (config::misc::caveBright) driver::wpm<float>(3.f, TOD_DayParameters + 0x50);

        float lastUpdate = driver::rpm<float>(TOD_Sky + 0x234);
        if (lastUpdate < updateInterval && ((driver::rpm<float>(TOD_NightParameters + 0x50) == 3.f) || (driver::rpm<float>(TOD_DayParameters + 0x50) == 3.f)))
        {
            driver::wpm<float>(9999999.f, TOD_AmbientParameters + 0x18);
            frozen_ambient = true;
        }
    }

    ////does not work
    //void sky()
    //{
    //    uint64_t TOD_CloudParamaters = driver::rpm<uint64_t>(TOD_Sky + 0x78, 2);
    //    if (!TOD_CloudParamaters) return;
    //    uint64_t TOD_AtmosphereParameters = driver::rpm<uint64_t>(TOD_Sky + 0x48, 2);
    //    if (!TOD_AtmosphereParameters) return;
    //    driver::wpm<float>(2.f, TOD_AtmosphereParameters + 0x10, 2); // retro effect //def 1,f
    //    driver::wpm<float>(3.f, TOD_AtmosphereParameters + 0x18, 2); //brightnesss def 1.f
    //    driver::wpm<float>(2.f, TOD_AtmosphereParameters + 0x1C, 2); //blue epicness def 1.350f
    //    
    //    //driver::wpm<float>(-1.f, TOD_CloudParamaters + 0x20, 2); //purple/yellow clouds can go into minus def 1,f
    //  //  driver::wpm<float>(2.f, TOD_CloudParamaters + 0x30, 2);//same as x20 except goes into white then + def1,f
    //}
}