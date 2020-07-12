#include "tarkovread.h"

#include "GameSpecific/Tarkov/Tarkov.h"
#include "GameSpecific/Tarkov/aimbot.h"

float MAX_RENDER_DISTANCE = 500.f;


void toWinPos(Vector2f& screenPos, float width, float height)
{
    screenPos.x = (screenPos.x + width) / 2;
    screenPos.y = (-screenPos.y + height) / 2;
}


bool TarkovReader::InGame()
{
    return this->game->GetGameStatus().IsGamePlaying();
}

void TarkovReader::fillBones(const Matrix4f& CameraMatrix, TarkovPlayerBones& playerBones, TarkovSkeletonRoot& skeletonRoot,
               float width, float height, Vector2f* LocalScreenPos, ESPObject& Object)
{
    Object.drawBones = true;

    Vector2f pRPVect;
    WorldToScreen(CameraMatrix, playerBones.GetRightPalmPosition(), pRPVect, width, height);
    pRPVect += *LocalScreenPos;
    toWinPos(pRPVect, width, height);

    Vector2f PLPVect;
    WorldToScreen(CameraMatrix, playerBones.GetLeftPalmPosition(), PLPVect, width, height);
    PLPVect += *LocalScreenPos;
    toWinPos(PLPVect, width, height);

    Vector2f PLShVect;
    WorldToScreen(CameraMatrix, playerBones.GetLeftShoulderPosition(), PLShVect, width, height);
    PLShVect += *LocalScreenPos;
    toWinPos(PLShVect, width, height);

    Vector2f PLRShVect;
    WorldToScreen(CameraMatrix, playerBones.GetRightShoulderPosition(), PLRShVect, width, height);
    PLRShVect += *LocalScreenPos;
    toWinPos(PLRShVect, width, height);

    Vector2f PLNeckVect;
    WorldToScreen(CameraMatrix, playerBones.GetNeckPosition(), PLNeckVect, width, height);
    PLNeckVect += *LocalScreenPos;
    toWinPos(PLNeckVect, width, height);

    Vector2f PLCentrVect;
    WorldToScreen(CameraMatrix, playerBones.GetPelvisPosition(), PLCentrVect, width, height);
    PLCentrVect += *LocalScreenPos;
    toWinPos(PLCentrVect, width, height);

    Vector2f PLRFootVect;
    WorldToScreen(CameraMatrix, playerBones.GetKickingFootPosition(), PLRFootVect, width, height);
    PLRFootVect += *LocalScreenPos;
    toWinPos(PLRFootVect, width, height);

    Vector2f PLLFootVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(18), PLLFootVect, width, height);
    PLLFootVect += *LocalScreenPos;
    toWinPos(PLLFootVect, width, height);

    Vector2f PLLBowVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(91), PLLBowVect, width, height);
    PLLBowVect += *LocalScreenPos;
    toWinPos(PLLBowVect, width, height);

    Vector2f PLRBowVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(112), PLRBowVect, width, height);
    PLRBowVect += *LocalScreenPos;
    toWinPos(PLRBowVect, width, height);

    Vector2f PLLKneeVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(17), PLLKneeVect, width, height);
    PLLKneeVect += *LocalScreenPos;
    toWinPos(PLLKneeVect, width, height);

    Vector2f PLRKneeVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(22), PLRKneeVect, width, height);
    PLRKneeVect += *LocalScreenPos;
    toWinPos(PLRKneeVect, width, height);

    Object.bones[0] = std::make_pair(PLNeckVect, PLCentrVect);
    Object.bones[1] = std::make_pair(PLShVect, PLLBowVect);
    Object.bones[2] = std::make_pair(PLRShVect, PLRBowVect);
    Object.bones[3] = std::make_pair(PLLBowVect, PLPVect);
    Object.bones[4] = std::make_pair(PLRBowVect, pRPVect);
    Object.bones[5] = std::make_pair(PLRShVect, PLShVect);
    Object.bones[6] = std::make_pair(PLLKneeVect, PLCentrVect);
    Object.bones[7] = std::make_pair(PLRKneeVect, PLCentrVect);
    Object.bones[8] = std::make_pair(PLLKneeVect, PLLFootVect);
    Object.bones[9] = std::make_pair(PLRKneeVect, PLRFootVect);
}


void TarkovReader::GetPlayers(ESPObjectArray *a, float width, float height, bool use_aimbot)
{
    WinProcess* GameProcess = game->GetWinProc();
    Matrix4f CameraMatrix = game->GetCameraMatrix();
    Vector3f CameraPosition = game->GetCameraLocation();
    std::vector<TarkovPlayer*> Players = game->GetPlayerList();

    clearArray(a);

    auto *LocalScreenPos = new Vector2f;
    WorldToScreen(CameraMatrix, CameraPosition, *LocalScreenPos, width, height);
    Vector3f meme;

    TarkovPlayer* pMyself = game->GetLocalPlayer();
    if (pMyself == nullptr)
        return;
    TarkovPlayer myself = *pMyself;
    std::string myGroupId = myself.GetPlayerProfile().GetPlayerInfo().GetGroupID().GetString();
    Vector3f myPosition = myself.GetPlayerBody().GetSkeletonRoot().GetLocationMatrixTest();
    Vector3f myFireportPosition = myself.GetPlayerHandsController().GetFireportPosition();

    TarkovMovementContext movement = myself.GetMovementContext();

    Vector3f localView;
    localView.x = movement.GetViewAngles1().x;
    localView.y = movement.GetViewAngles1().y;

    volatile float fov;
    float bestFov = 999.0f;
    uintptr_t chosenPlayer = 0;
    Vector3f chosenPlayerAngle;

    for (TarkovPlayer* PlayerPtr : Players)
    {
        TarkovPlayer Player = *PlayerPtr;
        if (Player == myself)
        {
            Player.infiniteStam();
            TarkovProceduralWeaponAnimation w = Player.GetProceduralWeaponAnimation();
            w.noRecoil();
            continue;
        }
        Vector3f PlayerPosition = Player.GetPlayerBody().GetSkeletonRoot().GetLocationMatrixTest();
        float distance = sqrt(pow(myPosition.x - PlayerPosition.x, 2) + pow(myPosition.y - PlayerPosition.y, 2) + pow(myPosition.z - PlayerPosition.z, 2) );

        if (distance >= MAX_RENDER_DISTANCE)
            continue;

        Vector2f *ScreenPos = new Vector2f;
        Vector2f *HeadScreenPos = new Vector2f;
        Vector3f Offset = Vector3f(0, 1, 0);

        bool Render = WorldToScreen(CameraMatrix, PlayerPosition + Offset, *ScreenPos, width, height);
        if (!Render)
            continue;
        *ScreenPos += *LocalScreenPos;

        TarkovPlayerBones playerBones = Player.GetPlayerBody().GetPlayerBones();
        TarkovSkeletonRoot skeletonRoot = Player.GetPlayerBody().GetSkeletonRoot();
        Vector3f headPosition = playerBones.GetHeadPosition();
        std::string groupId = Player.GetPlayerProfile().GetPlayerInfo().GetGroupID().GetString();
        bool is_friend = (groupId != "" && groupId == myGroupId && !Player.IsScav());
        if (distance < 150.f && use_aimbot && !is_friend)
        {
            Vector3f aimAngle = CalculateAngle( myFireportPosition, headPosition );
            fov = AngleFOV( localView, aimAngle );
            if( fov < bestFov){
                bestFov = fov;
                chosenPlayer = Player.Address;
                chosenPlayerAngle = aimAngle;
            }
        }

        WorldToScreen(CameraMatrix, headPosition, *HeadScreenPos, width, height);
        *HeadScreenPos += *LocalScreenPos;

        ESPObject Object;
        strcpy(Object.pName, (Player.IsScav() ? Player.IsPlayerScav() ? "Player Scav" : "Scav" : Player.GetPlayerProfile().GetPlayerInfo().GetPlayerName().GetString()).c_str());
        if (Player.IsScav())
        {
            Object.r = 255 / 255.f;
            Object.g = 255 / 255.f;
            Object.b = 0 / 255.f;
        } else
        {
            if (is_friend)
            {
                Object.r = 0 ;
                Object.g = 255 / 255.f;
                Object.b = 0 / 255.f;
            } else
            {
                if (groupId.empty())
                {
                    // Solo players
                    Object.r = 255/255.f ;
                    Object.g = 0 / 255.f;
                    Object.b = 0 / 255.f;
                }
                else {
                    auto t = teamColors.find(groupId);
                    color c{};
                    if (t == teamColors.end()) {
                        c = GetNewColor();
                        teamColors.insert(std::make_pair(groupId, c));
                    } else {
                        c = t->second;
                    }
                    Object.r = c.r / 255.f;
                    Object.g = c.g / 255.f;
                    Object.b = c.b / 255.f;
                }
            }
        }
        toWinPos(*ScreenPos, width, height);
        Object.x = ScreenPos->x;
        Object.y = ScreenPos->y;
        toWinPos(*HeadScreenPos, width, height);
        Object.xHead = HeadScreenPos->x;
        Object.yHead = HeadScreenPos->y;
        Object.inGameDistance = distance;
        Object.distance = (CameraPosition - PlayerPosition).length();
        Object.drawBones = false;

        if (distance < 100.f)
            fillBones(CameraMatrix, playerBones, skeletonRoot, width, height, LocalScreenPos, Object);

        insertArray(a, Object);

        delete ScreenPos;
        delete HeadScreenPos;
    }

    delete LocalScreenPos;

    if( !chosenPlayer || bestFov > 10.0f ){
        return;
    }

    GameProcess->Write<Vector2f>( movement.Address + 0x1d8,
                                  Vector2f(chosenPlayerAngle.x, chosenPlayerAngle.y) );
}


void TarkovReader::GetLoot(ESPObjectArray *a, float width, float height)
{
    return;
    Matrix4f CameraMatrix = game->GetCameraMatrix();
    Vector3f CameraPosition = game->GetCameraLocation();
    std::vector<TarkovLootItem*> Items = game->GetLootList();

    // do not clear array as there's players in it
    //clearArray(a);

    TarkovPlayer* pMyself = game->GetLocalPlayer();
    if (pMyself == nullptr)
        return;
    TarkovPlayer myself = *pMyself;
    Vector3f myPosition = myself.GetPlayerBody().GetSkeletonRoot().GetLocationMatrixTest();

    Vector2f *LocalScreenPos = new Vector2f;
    WorldToScreen(CameraMatrix, CameraPosition, *LocalScreenPos, width, height);

    for (TarkovLootItem* ItemPtr : Items)
    {
        TarkovLootItem Item = *ItemPtr;
        if (!Item.IsHighValue())
            continue;

        Vector2f *ScreenPos = new Vector2f;
        Vector3f LootLocation = Item.GetLootLocation();
        float distance = sqrt(pow(myPosition.x - LootLocation.x, 2) + pow(myPosition.y - LootLocation.y, 2) + pow(myPosition.z - LootLocation.z, 2) );
        if (distance > MAX_RENDER_DISTANCE)
            continue;
        bool Render = WorldToScreen(CameraMatrix, LootLocation, *ScreenPos, width, height);
        *ScreenPos += *LocalScreenPos;
        if (!Render)
            continue;

        ESPObject Object;
        strcpy(Object.pName, Item.GetLootName().c_str());
        Object.r = 0 / 255.f;
        Object.g = 125 / 255.f;
        Object.b = 255 / 255.f;
        Object.x = ScreenPos->x;
        Object.y = ScreenPos->y;
        Object.xHead = Object.x;
        Object.yHead = Object.y;
        Object.inGameDistance = distance;
        Object.distance = (CameraPosition - LootLocation).length();
        Object.drawBones = false;

        insertArray(a, Object);

        delete ScreenPos;
    }

    delete LocalScreenPos;
}
