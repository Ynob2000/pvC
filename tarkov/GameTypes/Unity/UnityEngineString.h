#pragma once
#include "Includes.h"

class UnityEngineString
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    UnityEngineString(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    int32_t GetStringSize()
    {
        return GameProcess->Read<int32_t>(Address + 0x10);
    }

    std::string GetUnicodeString(uint64_t addr, int stringLength)
    {
        char16_t wcharTemp[64] = { '\0' };
        GameProcess->Read(addr, wcharTemp, stringLength * 2);
        std::string u8_conv = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(wcharTemp);
        return u8_conv;
    }

    std::string GetString()
    {
        std::string result;

        int32_t StringSize = GetStringSize();
        result.resize(StringSize);

        return this->GetUnicodeString(Address + 0x14, StringSize);
    }
};