#pragma once
#include "Includes.h"

class TarkovLootItem
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovLootItem(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    uint64_t GetBasicInfo()
    {
        uint64_t m_lItemProfile = GameProcess->Read<uint64_t>(Address + 0x10);

        uint64_t m_lItemBasicInfo = GameProcess->Read<uint64_t>(m_lItemProfile + 0x30);
        return m_lItemBasicInfo;
    }

    void ReplaceStringInPlace(std::string &subject, const std::string &search, const std::string &replace)
    {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos)
        {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }

    bool IsAlphabet(char c)
    {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '(' || c == ')' || c == '_';
    }

    bool IsCorpse()
    {
        std::string LootName = GetLootName(false);

        bool IsPlayerCorpse = LootName.find("Observed_PlayerSuperior") != std::string::npos;
        bool IsCorpse = LootName.find("LootCorpse_PlayerSuperior") != std::string::npos;
        bool IsScavCorpse = LootName.find("Bot") != std::string::npos;

        return (IsPlayerCorpse || IsCorpse) && !IsScavCorpse;
    }

    bool IsHighValue()
    {
        std::string LootName = GetLootName(false);

        bool IsLedX = LootName.find("_transilluminator") != std::string::npos;
        bool IsCardA = LootName.find("key_") != std::string::npos;
        bool IsLab = LootName.find("lab") != std::string::npos;
        bool IsIntel = LootName.find("info_intelligence") != std::string::npos;
        bool IsGPU = LootName.find("video_card") != std::string::npos;
        bool IsEngine = LootName.find("barter_electr_engine") != std::string::npos;
        bool IsTetriz = LootName.find("barter_electr_tetriz") != std::string::npos;
        bool IsCorpseA = IsCorpse();

        return IsLedX || IsCardA || IsLab || IsCorpseA || IsIntel || IsGPU || IsEngine || IsTetriz;
    }

    std::string GetLootName(bool Filtered = true)
    {
        uint64_t m_lItemBasicInfo = GetBasicInfo();

        std::string result;
        uint64_t m_lItemName = GameProcess->Read<uint64_t>(m_lItemBasicInfo + 0x60);

        for (int i = 0; i <= 64; i++)
        {
            char CurrentChar = GameProcess->Read<char>(m_lItemName + i);
            // std::tolower()

            if (CurrentChar == '\0')
                break;

            if (IsAlphabet(CurrentChar))
                result.push_back(CurrentChar);
        }

        if (Filtered)
        {
            ReplaceStringInPlace(result, "item_", "");
            ReplaceStringInPlace(result, "(Clone)", "");
            ReplaceStringInPlace(result, "Observed_PlayerSuperior", "PLAYER CORPSE");
            ReplaceStringInPlace(result, "LootCorpse_PlayerSuperior", "CORPSE");
            ReplaceStringInPlace(result, "Bot", "SCAV CORPSE");
        }

        return result;
    }

    Vector3f GetLootLocation()
    {
        uint64_t m_lItemBasicInfo = GetBasicInfo();

        uint64_t m_lItemPositionPtr = ReadPtrChain(GameProcess, m_lItemBasicInfo, {0x30, 0x8, 0x38});
        Vector3f m_lItemPosition = GameProcess->Read<Vector3f>(m_lItemPositionPtr + 0xB0);

        return m_lItemPosition;
    }
};