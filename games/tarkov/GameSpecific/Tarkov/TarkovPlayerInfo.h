#pragma once
#include "Includes.h"
#include "tarkov/GameTypes/Unity/UnityEngineString.h"

enum EMemberCategory
{
    Default = 0,
    Developer = 1,
    UniqueId = 2,
    PressAccount = 3,
    Trader = 8,
    System = 16,
    ChatModerator = 32,
    ChatModeratorWithPermanentBan = 64,
    Emissar = 98,
    UnitTest = 128
};

class TarkovPlayerInfo
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    std::map<EMemberCategory, std::string> MemberString;

    uint64_t tablePlayerExperience[75] = {1000,
                                          3743,
                                          7742,
                                          12998,
                                          19492,
                                          27150,
                                          36001,
                                          46026,
                                          57124,
                                          69350,
                                          82686,
                                          99500,
                                          119424,
                                          142477,
                                          168760,
                                          197979,
                                          230024,
                                          264490,
                                          301534,
                                          340696,
                                          382188,
                                          426190,
                                          473090,
                                          524580,
                                          580660,
                                          641330,
                                          706590,
                                          776440,
                                          850840,
                                          929870,
                                          1013490,
                                          1104454,
                                          1202762,
                                          1308414,
                                          1421410,
                                          1541750,
                                          1669434,
                                          1804462,
                                          1946834,
                                          2096550,
                                          2253610,
                                          2420768,
                                          2598024,
                                          2785378,
                                          2982830,
                                          3190380,
                                          3408028,
                                          3635774,
                                          3873618,
                                          4121560,
                                          4379600,
                                          4651410,
                                          4936990,
                                          5236340,
                                          5549460,
                                          5872910,
                                          6235021,
                                          6604557,
                                          6991535,
                                          7398709,
                                          7828833,
                                          8286497,
                                          8780881,
                                          9330345,
                                          9953249,
                                          10713853,
                                          11749857,
                                          13198961,
                                          23198961
    };

    TarkovPlayerInfo(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;

        MemberString[EMemberCategory::Default] = "Normal";
        MemberString[EMemberCategory::Developer] = "Developer";
        MemberString[EMemberCategory::UniqueId] = "UniqueID";
        MemberString[EMemberCategory::PressAccount] = "Press";
        MemberString[EMemberCategory::Trader] = "Trader";
        MemberString[EMemberCategory::System] = "System";
        MemberString[EMemberCategory::ChatModerator] = "Chat Mod";
        MemberString[EMemberCategory::ChatModeratorWithPermanentBan] = "Chat Mod w/Perm Ban";
        MemberString[EMemberCategory::Emissar] = "Emissar";
        MemberString[EMemberCategory::UnitTest] = "Unit Test";
    }

    UnityEngineString GetPlayerName()
    {
        return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10));
    }

    UnityEngineString GetGroupID()
    {
        return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(Address + 0x18));
    }

    int32_t GetPlayerUnixCreationTime()
    {
        return GameProcess->Read<int32_t>(Address + 0x54);
    }

    EMemberCategory GetPlayerAccountType()
    {
        return (EMemberCategory)GameProcess->Read<int32_t>(Address + 0x60);
    }

    int GetLevel()
    {
        auto resultLevel = 1;
        int32_t exp = GameProcess->Read<int32_t>(Address + 0x64);
        for (size_t currentLevel = 0; currentLevel < sizeof(tablePlayerExperience) - 1; currentLevel++)
        {
            auto currentLevelExp = tablePlayerExperience[currentLevel];

            if (exp < currentLevelExp)
            {
                resultLevel = currentLevel + 1;
                break;
            }
        }

        return resultLevel;
    }
};