#pragma once
#include "Includes.h"

class TarkovPlayerBones
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovPlayerBones(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    Vector3f GetBodyPartPosition(int offset, bool bifacial)
    {
        uint64_t object = GameProcess->Read<uint64_t>(Address + offset);
        if (bifacial)
            object = GameProcess->Read<uint64_t>(object + 0x10);

        return get_transform_position(GameProcess, object);
    }

    Vector3f GetLeftPalmPosition()
    {
        return GetBodyPartPosition(0x60, false);
    }

    Vector3f GetRightPalmPosition()
    {
        return GetBodyPartPosition(0x68, false);
    }

    Vector3f GetLeftShoulderPosition()
    {
        return GetBodyPartPosition(0xe0, true);
    }

    Vector3f GetRightShoulderPosition()
    {
        return GetBodyPartPosition(0xe8, true);
    }

    Vector3f GetNeckPosition()
    {
        return GetBodyPartPosition(0x30, false);
    }

    Vector3f GetPelvisPosition()
    {
        return GetBodyPartPosition(0x110, true);
    }

    Vector3f GetKickingFootPosition()
    {
        return GetBodyPartPosition(0xb8, false);
    }

    Vector3f GetHeadPosition()
    {
        return GetBodyPartPosition(0xd8, true);
    }
};
