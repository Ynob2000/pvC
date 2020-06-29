#pragma once
#include "GameTypes/Unity/FPSCamera.h"

class TarkovFPSCamera : public FPSCamera
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    bool IsValid;

    TarkovFPSCamera()
    {
        Address = 0x0;
        IsValid = false;
    }

    TarkovFPSCamera(WinProcess *GameProc, GameObject Object)
    {
        GameProcess = GameProc;
        Address = Object.GetComponent();
        IsValid = true;
    }

    Vector3f GetCameraCoordinates()
    {
        uint64_t Camera = ReadPtrChain(GameProcess, Address, {0x8, 0x38});
        return GameProcess->Read<Vector3f>(Camera + 0xB0);
    }
    
    Matrix4f GetCameraMatrix()
    {
        uint64_t Camera = GameProcess->Read<uint64_t>(Address + 0x18);
        return GameProcess->Read<Matrix4f>(Camera + 0xD8);
    }
};