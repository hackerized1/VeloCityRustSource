#pragma once
#include "fnv.h"
#include "xor.h"
#include "driver/include.h"
#include "sdk.h"
#include "globals.h"
#include "item.h"
#include "defs.h"
#include <algorithm>
#include "config.h"

namespace weapon
{
    float getBulletVelocity(uint32_t weaponName, float dist)
    {
        if (weaponName == fnv::hash(xorstr("bow.hunting")))
        {
            if (dist <= 41.f)
            {
                return 60.f;
            }
            else
                if (dist <= 82.f)
                {
                    return  58.f;
                }
                else
                    if (dist <= 102.f)
                    {
                        return  57.5f;
                    }
                    else
                        if (dist <= 112.f)
                        {
                            return  57.3f;
                        }
                        else
                            if (dist <= 127.f)
                            {
                                return  57.f;
                            }
                            else
                                if (dist <= 146.f)
                                {
                                    return  56.5f;
                                }
                                else
                                    if (dist <= 153.f)
                                    {
                                        return  56.3f;
                                    }
                                    else
                                        if (dist <= 163.f)
                                        {
                                            return  56.f;
                                        }
                                        else
                                            if (dist <= 172.f)
                                            {
                                                return  55.7f;
                                            }
                                            else
                                                if (dist <= 178.f)
                                                {
                                                    return  55.5f;
                                                }
                                                else
                                                    if (dist <= 184.f)
                                                    {
                                                        return  55.3f;
                                                    }
                                                    else
                                                        if (dist <= 189.f)
                                                        {
                                                            return  55.1f;
                                                        }
                                                        else
                                                            if (dist <= 196.f)
                                                            {
                                                                return  54.9f;
                                                            }
                                                            else
                                                                if (dist <= 201.f)
                                                                {
                                                                    return  54.7f;
                                                                }
                                                                else
                                                                    if (dist <= 206.f)
                                                                    {
                                                                        return  54.5f;
                                                                    }
                                                                    else
                                                                        if (dist <= 210.f)
                                                                        {
                                                                            return  54.3f;
                                                                        }
                                                                        else
                                                                            if (dist <= 215.f)
                                                                            {
                                                                                return 54.1f;
                                                                            }
                                                                            else
                                                                                if (dist <= 220.f)
                                                                                {
                                                                                    return  53.9f;
                                                                                }
                                                                                else
                                                                                    if (dist <= 225.1f)
                                                                                    {
                                                                                        return 53.7f;
                                                                                    }
                                                                                    else
                                                                                        if (dist <= 230.1f)
                                                                                        {
                                                                                            return  53.5f;
                                                                                        }
                                                                                        else
                                                                                            if (dist <= 233.1f)
                                                                                            {
                                                                                                return  53.3f;
                                                                                            }
                                                                                            else
                                                                                                if (dist <= 237.1f)
                                                                                                {
                                                                                                    return  53.1f;
                                                                                                }
                                                                                                else
                                                                                                    if (dist <= 241.1f)
                                                                                                    {
                                                                                                        return  52.9f;
                                                                                                    }
                                                                                                    else
                                                                                                        if (dist <= 244.1f)
                                                                                                        {
                                                                                                            return  52.7f;
                                                                                                        }
                                                                                                        else
                                                                                                            if (dist <= 248.1f)
                                                                                                            {
                                                                                                                return  52.5f;
                                                                                                            }
                                                                                                            else
                                                                                                                if (dist <= 252.1f)
                                                                                                                {
                                                                                                                    return  52.3f;
                                                                                                                }
                                                                                                                else
                                                                                                                    if (dist <= 255.1f)
                                                                                                                    {
                                                                                                                        return  52.1f;
                                                                                                                    }
                                                                                                                    else
                                                                                                                        if (dist <= 500.f)
                                                                                                                        {
                                                                                                                            return 50.f;
                                                                                                                        }
        }

        else if (weaponName == fnv::hash(xorstr("crossbow")))
        {
            if (dist <= 83.f)
            {
                return 90.f;
            }
            else
                if (dist <= 123.f)
                {
                    return 88.f;
                }
                else
                    if (dist <= 223.f)
                    {
                        return 86.f;
                    }
                    else
                        if (dist <= 254.f)
                        {
                            return 85.5f;
                        }
                        else
                            if (dist <= 285.f)
                            {
                                return 85.f;
                            }
                            else
                                if (dist <= 500.f)
                                {
                                    return 84.7f;
                                }
        }

        else if (weaponName == fnv::hash(xorstr("rifle.ak")))               return 374.9f;
        else if (weaponName == fnv::hash(xorstr("rifle.semiauto")))         return 374.9f;
        else if (weaponName == fnv::hash(xorstr("rifle.lr300")))            return 374.9f;
        else if (weaponName == fnv::hash(xorstr("rifle.m39")))              return 374.9f;

        else if (weaponName == fnv::hash(xorstr("bow.hunting")))            return 50.f;
        else if (weaponName == fnv::hash(xorstr("pistol.nailgun")))         return 60.f;

        else if (weaponName == fnv::hash(xorstr("rifle.bolt")))             return 656.0f;

        else if (weaponName == fnv::hash(xorstr("pistol.semiauto")))        return 300.f;
        else if (weaponName == fnv::hash(xorstr("pistol.python")))          return 300.f;
        else if (weaponName == fnv::hash(xorstr("pistol.revolver")))        return 300.f;
        else if (weaponName == fnv::hash(xorstr("smg.thompson")))           return 300.f;
        else if (weaponName == fnv::hash(xorstr("pistol.m92")))             return 300.f;
        else if (weaponName == fnv::hash(xorstr("pistol.python")))          return 300.f;

        else if (weaponName == fnv::hash(xorstr("rifle.l96")))              return 1125.0f;
        else if (weaponName == fnv::hash(xorstr("lmg.m249")))               return 487.4f;

        else if (weaponName == fnv::hash(xorstr("smg.mp5")))                return 240.f;
        else if (weaponName == fnv::hash(xorstr("smg.2")))                  return 240.f;

        else if (weaponName == fnv::hash(xorstr("shotgun.pump")))           return 100.f;
        else if (weaponName == fnv::hash(xorstr("shotgun.spas12")))         return 100.f;
        else if (weaponName == fnv::hash(xorstr("shotgun.waterpipe")))      return 100.f;
        else if (weaponName == fnv::hash(xorstr("shotgun.doublebarrel")))   return 100.f;

        return 300.f;
    }

    float getAttachmentMultiplier()
    {
        uintptr_t childrenList = driver::rpm<uintptr_t>(globals::objects::localActiveWep.heldEntity + 0x40);
        if (!childrenList) return 1.f;

        childrenList = driver::rpm<uintptr_t>(childrenList + 0x10);
        if (!childrenList) return 1.f;

        int childrenCount = driver::rpm<int>(childrenList + 0x18);
        for (int h = 0; h < childrenCount; h++)
        {
            uintptr_t childEntity = driver::rpm<uintptr_t>(childrenList + 0x20 + (h * 0x8));
            if (!childEntity) continue;

            uintptr_t unk1 =  driver::rpm<uintptr_t>(childEntity);
            if (!unk1) continue;

            uintptr_t unk2 = driver::rpm<uintptr_t>(unk1 + 0x10);
            if (!unk2) continue;

            std::string ent_type = driver::rpm<structures::str_2>(unk2).buffer;
            if (fnv::hash_runtime(ent_type.c_str()) == fnv::hash(xorstr("ProjectileWeaponMod")) && driver::rpm<float>(childEntity + 0x188))
            {
                return driver::rpm<float>(childEntity + 0x188);
            }
        }

        return 1.f;
    }


    float getBulletDrag()
    {
        uint64_t primaryMagazine = driver::rpm<uint64_t>(globals::objects::localActiveWep.heldEntity + 0x2B8);
        if (!primaryMagazine) return  1.f;

        uint64_t ammoType = driver::rpm<uint64_t>(primaryMagazine + 0x20);
        if (!ammoType) return  1.f;

        uint64_t shortName = driver::rpm<uint64_t>(ammoType + 0x20);
        if (!shortName) return  1.f;

        uint32_t bulletType = fnv::hash_runtime(sdk::getString(driver::rpm<structures::str>(shortName)).c_str());
        if (!bulletType) return  1.f;

        if (bulletType == fnv::hash(xorstr("arrow.hv"))) return  1.9f;
        else if (bulletType == fnv::hash(xorstr("arrow.bone"))) return 0.8f;
        else  if (bulletType == fnv::hash(xorstr("arrow.fire"))) return  1.0f;
        else if (bulletType == fnv::hash(xorstr("ammo.rifle.hv"))) return  1.0;
        else  if (bulletType == fnv::hash(xorstr("ammo.rifle.explosive"))) return  1.25f;
        //else if (bulletType == fnv::hash(xorstr("ammo.nailgun.nails"))) return  0.75f;

        return 1.f;
    }

    bool isWeapon(uint32_t hash)
    {

        if (hash == fnv::hash(xorstr("rifle.ak")) ||
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
            hash == fnv::hash(xorstr("pistol.m92")))
        {

            return 1;
        }


        return 0;

    }

    void noSway()
    {

        if (!globals::objects::localActiveWep.heldEntity) return;

        driver::wpm<float>(0.f, globals::objects::localActiveWep.heldEntity + 0x2D0);
        driver::wpm<float>(0.f, globals::objects::localActiveWep.heldEntity + 0x2D4);

    }
    
    void rapidFire()
    {

        if (!globals::objects::localActiveWep.heldEntity) return;

        driver::wpm<bool>(1, globals::objects::localActiveWep.heldEntity + 0x288);

    }

    void perfectRecoil()
    {

        if (!globals::objects::localActiveWep.heldEntity) return;

        float max = config::weapon::perfectRecoilMax;
        float min = std::clamp<float>(max - 1, 0, 99999);

        uintptr_t recoil = driver::rpm<uintptr_t>(globals::objects::localActiveWep.heldEntity + 0x2D8);

        structures::NoRecoil.minYaw = -min;
        structures::NoRecoil.maxYaw = -max;
        structures::NoRecoil.minPitch = -min;
        structures::NoRecoil.maxPitch = -max;

        driver::wpm<structures::NoRecoil_t>(structures::NoRecoil, recoil + 0x18);

    }


    void noSpread()
    {

        if (!globals::objects::localActiveWep.heldEntity) return;

        structures::NoAimcone = driver::rpm<structures::NoAimcone_t>(globals::objects::localActiveWep.heldEntity + 0x2E8);

        structures::NoAimcone.m = -1.f;
        structures::NoAimcone.m1 = -1.f;
        structures::NoAimcone.m2 = -1.f;
        structures::NoAimcone.m3 = -1.f;
        structures::NoAimcone.m4 = -1.f;
        structures::NoAimcone.m5 = -1.f;

        driver::wpm<structures::NoAimcone_t>(structures::NoAimcone, globals::objects::localActiveWep.heldEntity + 0x2E8);

    }

    void changeRecoil(float percentage)
    {

        math::vector3 recoil = driver::rpm<math::vector3>(globals::objects::localPlayer.input + 0x64);
        math::vector3 newRecoil = (recoil / 100.f) * percentage;

        driver::wpm<math::vector3>(newRecoil, globals::objects::localPlayer.input + 0x64);

    }

    void instaEoka()
    {
        if (!globals::objects::localActiveWep.heldEntity) return;

        driver::wpm<float>(100.f, globals::objects::localActiveWep.heldEntity + 0x360);
    }

    void instaBow()
    {

        if (!globals::objects::localActiveWep.heldEntity) return;

        driver::wpm<bool>(1, globals::objects::localActiveWep.heldEntity + 0x360);

    }

    void instaCompoundBow()
    {
        if (!globals::objects::localActiveWep.heldEntity) return;

        driver::wpm<int>(0.f, globals::objects::localActiveWep.heldEntity + 0x378);
    }

    namespace melee
    {
        void fatHand()
        {
            if (!globals::objects::localActiveWep.heldEntity) return;

            driver::wpm<float>(15.f, globals::objects::localActiveWep.heldEntity + 0x294);
        }

        void longHand()
        {
            if (!globals::objects::localActiveWep.heldEntity) return;

            driver::wpm<float>(4.5f, globals::objects::localActiveWep.heldEntity + 0x290);
        }

        void attackWhileSprint()
        {
            if (!globals::objects::localActiveWep.heldEntity) return;

            driver::wpm<int>(0, globals::objects::localActiveWep.heldEntity + 0x299);
        }
    }

    void handle()
    {
        uint32_t hashedWeapon = fnv::hash_runtime(sdk::getString(item::getShortname(globals::objects::localActiveWep)).c_str());
        if (hashedWeapon)
        {
            bool isWeapon = item::isWeapon(hashedWeapon);
            if (isWeapon)
            {
                if (config::weapon::noSpread) weapon::noSpread();
                if (config::weapon::noSway) weapon::noSway();
                if (config::weapon::rapidFire) weapon::rapidFire();
                if (config::weapon::perfectRecoil) weapon::perfectRecoil();
            }
            else if (hashedWeapon == fnv::hash(xorstr("pistol.eoka")))
            {
                if (config::weapon::instantEoka)  weapon::instaEoka();
            }
            else if (config::weapon::melee::longHand || config::weapon::melee::fatHand || config::weapon::melee::attackWhileSprint)
            {
                if (!isWeapon)
                {
                    if (item::getCategory(globals::objects::localActiveWep) == itemcategories::Tool)
                    {
                        if (config::weapon::melee::longHand) weapon::melee::longHand();
                        if (config::weapon::melee::fatHand) weapon::melee::fatHand();
                        if (config::weapon::melee::attackWhileSprint) weapon::melee::attackWhileSprint();
                    }
                }
            }

            if (hashedWeapon == fnv::hash(xorstr("bow.hunting")))
            {
                if (config::weapon::instantBow) weapon::instaBow();
            }
            else if (hashedWeapon == fnv::hash(xorstr("bow.compound")))
            {
                if (config::weapon::instantCompoundBow) weapon::instaCompoundBow();
            }
        }
    }
}