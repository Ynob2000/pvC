#pragma once
#include "GameTypes/Unity/GameObjectManager.h"
#include "TarkovGameWorld.h"
#include "TarkovFPSCamera.h"
#include "Includes.h"

const struct Offsets
{
    static const uint64_t GameObjectManager = 0x151A218;
} Offsets;

struct GameStatus
{
private:
    bool GameActive;
    bool GamePlaying;

    int32_t PlayerCount;

public:
    inline void SetGameActive_Intern(bool InGameActive) { GameActive = InGameActive; };
    inline void SetGamePlaying_Intern(bool InGamePlaying) { GamePlaying = InGamePlaying; };

    inline bool IsGameActive() { return GameActive; };
    inline bool IsGamePlaying() { return GamePlaying; };
};

class TarkovGame
{
protected:
    GameObjectManager GOM{};
    TarkovGameWorld LGW{};
    TarkovFPSCamera FPCamera{};

private:
    WinContext *ProcessContext;
    WinProcess *GameProcess;
    uint64_t ModuleBase;

    bool GameActive = false;
    bool InGame = false;
    GameStatus Status;

    Matrix4f CameraMatrix;
    Vector3f CameraLocation;
    TarkovPlayer* LocalPlayer;
    std::vector<TarkovPlayer*> PlayerList;
    std::vector<TarkovLootItem*> LootList;

public:
    inline GameStatus GetGameStatus() { return Status; };
    inline Matrix4f GetCameraMatrix() { return CameraMatrix; };
    inline Vector3f GetCameraLocation() { return CameraLocation; };
    inline TarkovPlayer* GetLocalPlayer() { return LocalPlayer; };
    inline std::vector<TarkovPlayer*> GetPlayerList() { return PlayerList; };
    inline std::vector<TarkovLootItem*> GetLootList() { return LootList; };
    inline WinProcess * GetWinProc() { return GameProcess; }; // remove

    TarkovGame(WinContext *ctx, WinProcess *InProcess, uint64_t InModuleBase)
    {
        ProcessContext = ctx;
        GameProcess = InProcess;
        ModuleBase = InModuleBase;

        GOM = GameObjectManager(GameProcess, GameProcess->Read<uint64_t>(ModuleBase + Offsets.GameObjectManager));
        LGW = GOM.FindGameObjectActive("GameWorld");
        FPCamera = GOM.FindGameObjectTagged<TarkovFPSCamera>("FPS Camera");

        Status = GameStatus();

        printf("Game Object Manager: %lx\tLocal Game World: %lx\n", GOM.Address, LGW.Address);
    }

    ~TarkovGame()
    {
        delete ProcessContext; /* rest of deallocation is handle inside ctx */
    }

    bool Tick()
    {
        Status.SetGameActive_Intern(GOM.Address != 0x0);
        Status.SetGamePlaying_Intern(LGW.IsValid && FPCamera.IsValid && LGW.GetPlayerCount() > 0);

        if (!Status.IsGameActive())
            return false;

        if (!Status.IsGamePlaying())
        {
            GOM = GameObjectManager(GameProcess, GameProcess->Read<uint64_t>(ModuleBase + Offsets.GameObjectManager));
            if (!LGW.IsValid || LGW.GetPlayerCount() == 0)
                LGW = GOM.FindGameObjectActive("GameWorld");

            if (!FPCamera.IsValid)
                FPCamera = GOM.FindGameObjectTagged<TarkovFPSCamera>("FPS Camera");
        }

        if (!Status.IsGameActive() || !Status.IsGamePlaying())
            return true;

        CameraMatrix = FPCamera.GetCameraMatrix();
        CameraLocation = FPCamera.GetCameraCoordinates();

        for (auto t : PlayerList)
            delete t;
        PlayerList = LGW.GetAllPlayers();
        LocalPlayer = LGW.GetLocalPlayer(PlayerList);
        for (auto t : LootList)
            delete t;
        LootList = LGW.GetAllLoot();

        return true;
    }
};

//Vector3f GetShootPos(Matrix4f& cameraMatrix, TarkovPlayer* tarkovPlayer)
//{
//    TarkovHandsController handsController = tarkovPlayer->GetPlayerHandsController();
//    // Get Fireport position
//    Vector3f fireportPos = handsController.GetFireportPosition();
//    Matrix4f t = cameraMatrix.transpose();
//    Vector3f forward = { t.at(2, 0), t.at(2, 1), t.at(2, 2) };
//    return fireportPos + forward * 1.f;
//}
//
//void AimAtPos(WinProcess* GameProcess, Matrix4f& cameraMatrix, TarkovPlayer* localPlayer, const Vector3f& pos)
//{
//    Vector3f shootPos = GetShootPos(cameraMatrix, localPlayer);
//    Vector3f axis = shootPos.crossProduct(pos);
//    axis.normalize();
//    float angle = acos(shootPos.dotProduct(pos) / (shootPos.length() * pos.length()));
//
//    Vector3f eulerAngles = toEuler(axis, angle);
//    if (eulerAngles.x > 180.f)
//    {
//        eulerAngles.x -= 360.f;
//    }
//    TarkovMovementContext movementContext = localPlayer->GetMovementContext();
//    //1d0 : vector2_0(type : UnityEngine.Vector2)
//    //1d8 : vector2_1(type : UnityEngine.Vector2)
//    //1e0 : vector2_2(type : UnityEngine.Vector2)
//    Vector2f toWrite = Vector2f(eulerAngles.y, eulerAngles.x);
//    // Rotate like pro
//    GameProcess->Write<Vector2f>(movementContext.Address + 0x1d8, toWrite);
//}
//
//void AimBot(WinProcess* GameProcess, const Vector3f& position, Matrix4f& cameraMatrix, TarkovPlayer* localPlayer, const Vector3f& vector)
//{
//    //  if (player.GroupId != DrawingPlayers.localgroup || DrawingPlayers.localgroup == "" || DrawingPlayers.localgroup == "0" || DrawingPlayers.localgroup == null)
//    //  {
//    if (CalcInFov(position, cameraMatrix, vector) <= 3.0f) // AIM FOV
//    {
//        //float travelTime = dist / local_player.Weapon.CurrentAmmoTemplate.InitialSpeed;
//        //vector.x += (player.Velocity.x * travelTime);
//        //vector.y += (player.Velocity.y * travelTime);
//        AimAtPos(GameProcess, cameraMatrix, localPlayer, vector);
//    }
//    //  }
//}
