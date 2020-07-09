#pragma once
#include "Includes.h"
#include "tarkov/GameTypes/Unity/UnityEngineString.h"

class TarkovHandsController
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovHandsController(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    UnityEngineString GetActiveWeaponName()
    {
        uint64_t ActiveWeapon = GameProcess->Read<uint64_t>(Address + 0x50);
        uint64_t WeaponTemplate = GameProcess->Read<uint64_t>(ActiveWeapon + 0x20);
        UnityEngineString WeaponName = UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(WeaponTemplate + 0x58));
//AmmoTemplate + 0x174] initial speed
        return WeaponName;
    }

    Vector3f GetFireportPosition()
    {
        uint64_t Fireport = GameProcess->Read<uint64_t>(Address + 0xd0);  // Bifacial transform
        uint64_t transform = GameProcess->Read<uint64_t>(Fireport + 0x10);  // Original
        return get_transform_position(GameProcess, transform);
    }


};