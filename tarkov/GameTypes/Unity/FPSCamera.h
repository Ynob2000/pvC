#pragma once
#include "Includes.h"
#include "GameTypes/Unity/GameObject.h"

class FPSCamera
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    bool IsValid;

    FPSCamera()
    {
        Address = 0x0;
        IsValid = false;
    }

    FPSCamera(WinProcess *GameProc, GameObject Object)
    {
        GameProcess = GameProc;
        Address = Object.GetComponent();
        IsValid = true;
    }
};