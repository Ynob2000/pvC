#pragma once
#include "Includes.h"

class TarkovMovementContext
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovMovementContext(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    Vector2f GetViewAngles1()
    {
        return GameProcess->Read<Vector2f>(Address + 0x1E0);
    }

    Vector2f GetViewAngles2()
    {
        return GameProcess->Read<Vector2f>(Address + 0x1E8);
    }

    Vector3f GetLocalPosition()
    {
        return GameProcess->Read<Vector3f>(Address + 0x1F0);
    }
};