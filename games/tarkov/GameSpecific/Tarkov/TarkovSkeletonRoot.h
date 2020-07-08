#pragma once

#include <xmmintrin.h>
#include "Includes.h"
#include "TarkovSkeletonRoot.h"

class TarkovSkeletonRoot
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovSkeletonRoot(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    uint64_t getBoneEnum()
    {
        return GameProcess->Read<uint64_t>(Address + 0x28);
    }

    Vector3f GetLocationMatrixTest(int bone_id = 0)
    {
        uint64_t m_BoneEnum = getBoneEnum();
        uint64_t m_TransformArray = GameProcess->Read<uint64_t>(m_BoneEnum + 0x10);
        uint64_t m_TransformA = GameProcess->Read<uint64_t>(m_TransformArray + 0x20 + (bone_id * 0x8));
        return get_transform_position(GameProcess, m_TransformA);
    }
};


