#pragma once
#include "Includes.h"

class GameObject
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    GameObject(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    std::string GetGameObjectName()
    {
        std::string result;
        uint64_t GameObjectNameAddr = GameProcess->Read<uint64_t>(Address + 0x60);

        for (int i = 0; i <= 50; i++)
        {
            char CurrentChar = GameProcess->Read<char>(GameObjectNameAddr + i);

            if (CurrentChar == '\0')
                break;

            result.push_back(CurrentChar);
        }
        return result;
    }

    uint64_t GetComponent()
    {
        return GameProcess->Read<uint64_t>(Address + 0x30);
    }
};