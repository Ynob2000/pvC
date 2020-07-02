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

    Vector3f GetLocationMatrixTest()
    {
        uint64_t m_BoneEnum = getBoneEnum();
        uint64_t m_TransformArray = GameProcess->Read<uint64_t>(m_BoneEnum + 0x10);
        uint64_t m_TransformA = GameProcess->Read<uint64_t>(m_TransformArray + 0x20);
        uint64_t m_internalTransformA = GameProcess->Read<uint64_t>(m_TransformA + 0x10);
        uint64_t m_transformAccessReadOnlyData = GameProcess->Read<uint64_t>(m_internalTransformA + 0x38);

        uint64_t m_transformDataArray = GameProcess->Read<uint64_t>(m_transformAccessReadOnlyData + 0x18);
        //uint64_t m_transformDataIndices = GameProcess->Read<uint64_t>(m_transformAccessReadOnlyData + 0x20);

        Vector3f locMatrixDebug = GameProcess->Read<Vector3f>(m_transformDataArray); /* 0x40 between matrix's? */

        /*
        https://www.unknowncheats.me/forum/2332931-post688.html
        https://www.unknowncheats.me/forum/2432035-post995.html
        */
        return locMatrixDebug;
    }
};