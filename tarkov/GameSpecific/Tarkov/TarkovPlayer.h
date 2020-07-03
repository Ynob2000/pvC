#pragma once
#include "Includes.h"
#include "TarkovMovementContext.h"
#include "TarkovPlayerProfile.h"
#include "TarkovPlayerBody.h"
#include "TarkovHandsController.h"
#include "TarkovProceduralWeaponAnimation.h"

class TarkovPlayer
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    std::string ID;
    std::string NAME;

    TarkovPlayer(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;

        ID = GetPlayerProfile().GetPlayerID().GetString();
        NAME = GetPlayerProfile().GetPlayerInfo().GetPlayerName().GetString();
    }

    bool IsLocalPlayer()
    {
        return GameProcess->Read<uint64_t>(Address + 0x18) ? true : false;
    }

    bool IsScav()
    {
        int32_t Creation = GetPlayerProfile().GetPlayerInfo().GetPlayerUnixCreationTime();
        return Creation == 0;
    }

    bool IsPlayerScav()
    {
        return !IsScav() && NAME.find(" ") != std::string::npos;
    }

    bool operator==(TarkovPlayer &other)
    {
        return ID == other.ID;
    }

    TarkovMovementContext GetMovementContext()
    {
        return TarkovMovementContext(GameProcess, GameProcess->Read<uint64_t>(Address + 0x38));
    }

    TarkovPlayerBody GetPlayerBody()
    {
        return TarkovPlayerBody(GameProcess, GameProcess->Read<uint64_t>(Address + 0xA0));
    }

    TarkovPlayerProfile GetPlayerProfile()
    {
        return TarkovPlayerProfile(GameProcess, GameProcess->Read<uint64_t>(Address + 0x3E0));
    }

    TarkovHandsController GetPlayerHandsController()
    {
        return TarkovHandsController(GameProcess, GameProcess->Read<uint64_t>(Address + 0x418));
    }

    TarkovProceduralWeaponAnimation GetProceduralWeaponAnimation()
    {
        return TarkovProceduralWeaponAnimation(GameProcess, GameProcess->Read<uint64_t>(Address + 0x168));
    }

    void DebugDump()
    {
        printf("Player Dump\n");

        TarkovPlayerProfile PlayerProfile = GetPlayerProfile();
        printf("Player Address:\t%lx\n", this->Address);
        printf("Player Profile Address:\t%lx\n", PlayerProfile.Address);
        TarkovPlayerInfo PlayerInfo = PlayerProfile.GetPlayerInfo();
        printf("Player Info Address:\t%lx\n", PlayerInfo.Address);
        printf("Player Name:\t%s\n", PlayerInfo.GetPlayerName().GetString().c_str());

        printf("Player Created:\t%d\n", PlayerInfo.GetPlayerUnixCreationTime());

        printf("Player ID:\t%s\n", PlayerProfile.GetPlayerID().GetString().c_str());

        printf("Is Local?: %s\tIs Scav?:%s\n", IsLocalPlayer() ? "Yes" : "No", IsScav() ? "Yes" : "No");

        TarkovHealthInfo HealthInfo = PlayerProfile.GetPlayerHealth().GetHealthInfo();
        printf("Player Health(current/max):\t%f/%f\n", HealthInfo.GetHealth(), HealthInfo.GetMaxHealth());

        Vector3f PlayerCoordinates = GetMovementContext().GetLocalPosition();
        printf("Player Coordinates (x-y-z):\t%f-%f-%f\n", PlayerCoordinates.x, PlayerCoordinates.y, PlayerCoordinates.z);

        Vector2f ViewAngle1 = GetMovementContext().GetViewAngles1();
        printf("View Angle 1 (x-y):\t%f-%f\n", ViewAngle1.x, ViewAngle1.y);

        Vector2f ViewAngle2 = GetMovementContext().GetViewAngles2();
        printf("View Angle 2 (x-y):\t%f-%f\n", ViewAngle2.x, ViewAngle2.y);
    }
};
