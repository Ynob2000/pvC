#pragma once
#include "Includes.h"
#include "GameObject.h"

class GameWorld
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    bool IsValid;

    GameWorld()
    {
        Address = 0x0;
        IsValid = false;
    }

    GameWorld(WinProcess *GameProc, GameObject Object)
    {
        GameProcess = GameProc;
        Address = ReadPtrChain(GameProc, Object.GetComponent(), {0x18, 0x28});
        IsValid = true;
    }
};