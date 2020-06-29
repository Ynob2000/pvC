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

    Vector3f GetHeadPosition()
    {
        uint64_t headObject = GameProcess->Read<uint64_t>(Address + 0xd8);
        uint64_t transformHead = GameProcess->Read<uint64_t>(headObject + 0x10);
        uint64_t m_internalTransformA = GameProcess->Read<uint64_t>(transformHead + 0x10);
        uint64_t m_transformAccessReadOnlyData = GameProcess->Read<uint64_t>(m_internalTransformA + 0x38);

        uint64_t m_transformDataArray = GameProcess->Read<uint64_t>(m_transformAccessReadOnlyData + 0x18);
        //uint64_t m_transformDataIndices = GameProcess->Read<uint64_t>(m_transformAccessReadOnlyData + 0x20);

        Vector3f locMatrixDebug = GameProcess->Read<Vector3f>(m_transformDataArray); /* 0x40 between matrix's? */
        return locMatrixDebug;
    }
};
