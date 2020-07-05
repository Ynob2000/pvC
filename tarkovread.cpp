#include "tarkovread.h"

//#include "Includes.h"
#include "GameSpecific/Tarkov/Tarkov.h"
#include "GameSpecific/Tarkov/aimbot.h"

float MAX_RENDER_DISTANCE = 500.f;


bool GetTarkovGame(TarkovGame **Tarkov, const char *ProcessName, const char *ModuleName)
{
    FILE *out = stdout;
    pid_t pid;

#if (LMODE() == MODE_EXTERNAL())
    FILE *pipe = popen("pidof qemu-system-x86_64", "r");
    fscanf(pipe, "%d", &pid);
    pclose(pipe);
#else
    out = fopen("/tmp/testr.txt", "w");
    pid = getpid();
#endif

    try
    {
        //WinContext ctx(pid);
        WinContext *ctx = new WinContext(pid); /* de-allocation handled in TarkovGame() */
        ctx->processList.Refresh();
        //ctx->processList.FindProc(ProcessName);

        fprintf(out, "\nInitializing Game Offsets\n");
        bool ProcessFound, ModuleFound = false;

        for (auto &i : ctx->processList)
        {
            if (!strcasecmp(ProcessName, i.proc.name))
            {
                fprintf(out, "Game:\tLooping process %lx:%s\n", i.proc.dirBase, i.proc.name);
                ProcessFound = true;

                //fprintf(out, "\tExports:\n");
                for (auto &o : i.modules)
                {
                    fprintf(out, "\t%.8lx\t%.8lx\t%lx\t%s\n", o.info.baseAddress, o.info.entryPoint, o.info.sizeOfModule, o.info.name);
                    if (!strcmp(ModuleName, o.info.name))
                    {
                        ModuleFound = true;
                        *Tarkov = new TarkovGame(ctx, &i, o.info.baseAddress);
                        return true;
                    }
                }
            }
        }

        if (ProcessFound == false)
            fprintf(out, "Error finding process: %s\n", ProcessName);

        if (ModuleFound == false)
            fprintf(out, "Error finding module: %s\n", ModuleName);
    }
    catch (VMException &e)
    {
        fprintf(out, "Initialization error: %d\n", e.value);
    }

    return false;
}

void DestroyTarkovGame(TarkovGame *TarkovGame)
{
    delete TarkovGame;
}

bool TickTarkov(TarkovGame *Tarkov)
{
    if (Tarkov == nullptr)
        return false;

    return Tarkov->Tick();
}

bool TarkovInGame(TarkovGame *Tarkov)
{
    return Tarkov->GetGameStatus().IsGamePlaying();
}

bool initArray(TarkovESPArray *a, int initialSize)
{
    a->array = (TarkovESPObject *)malloc(initialSize * sizeof(TarkovESPObject));
    if (!a->array)
        return false;

    memset(a->array, 0, initialSize * sizeof(TarkovESPObject));

    a->used = 0;
    a->size = initialSize;
    return true;
}

void insertArray(TarkovESPArray *a, TarkovESPObject element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (TarkovESPObject *)realloc(a->array, a->size * sizeof(TarkovESPObject));
    }
    a->array[a->used++] = element;
}

void clearArray(TarkovESPArray *a)
{
    memset(a->array, 0, a->size * sizeof(TarkovESPObject));
    a->used = 0;
}

void freeArray(TarkovESPArray *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}


void fillBones(const Matrix4f& CameraMatrix, TarkovPlayerBones& playerBones, TarkovSkeletonRoot& skeletonRoot,
               int width, int height, Vector2f* LocalScreenPos, TarkovESPObject& Object)
{
    Object.drawBones = true;
    Vector2f pRPVect;
    WorldToScreen(CameraMatrix, playerBones.GetRightPalmPosition(), pRPVect, width, height);
    pRPVect += *LocalScreenPos;
    Vector2f PLPVect;
    WorldToScreen(CameraMatrix, playerBones.GetLeftPalmPosition(), PLPVect, width, height);
    PLPVect += *LocalScreenPos;
    Vector2f PLShVect;
    WorldToScreen(CameraMatrix, playerBones.GetLeftShoulderPosition(), PLShVect, width, height);
    PLShVect += *LocalScreenPos;
    Vector2f PLRShVect;
    WorldToScreen(CameraMatrix, playerBones.GetRightShoulderPosition(), PLRShVect, width, height);
    PLRShVect += *LocalScreenPos;
    Vector2f PLNeckVect;
    WorldToScreen(CameraMatrix, playerBones.GetNeckPosition(), PLNeckVect, width, height);
    PLNeckVect += *LocalScreenPos;
    Vector2f PLCentrVect;
    WorldToScreen(CameraMatrix, playerBones.GetPelvisPosition(), PLCentrVect, width, height);
    PLCentrVect += *LocalScreenPos;
    Vector2f PLRFootVect;
    WorldToScreen(CameraMatrix, playerBones.GetKickingFootPosition(), PLRFootVect, width, height);
    PLRFootVect += *LocalScreenPos;
    Vector2f PLLFootVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(18), PLLFootVect, width, height);
    PLLFootVect += *LocalScreenPos;
    Vector2f PLLBowVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(91), PLLBowVect, width, height);
    PLLBowVect += *LocalScreenPos;
    Vector2f PLRBowVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(112), PLRBowVect, width, height);
    PLRBowVect += *LocalScreenPos;
    Vector2f PLLKneeVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(17), PLLKneeVect, width, height);
    PLLKneeVect += *LocalScreenPos;
    Vector2f PLRKneeVect;
    WorldToScreen(CameraMatrix, skeletonRoot.GetLocationMatrixTest(22), PLRKneeVect, width, height);
    PLRKneeVect += *LocalScreenPos;

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


void GetTarkovPlayers(TarkovGame *Tarkov, TarkovESPArray *a, float width, float height, bool use_aimbot)
{
    WinProcess* GameProcess = Tarkov->GetWinProc();
    Matrix4f CameraMatrix = Tarkov->GetCameraMatrix();
    Vector3f CameraPosition = Tarkov->GetCameraLocation();
    std::vector<TarkovPlayer*> Players = Tarkov->GetPlayerList();

    clearArray(a);

    Vector2f *LocalScreenPos = new Vector2f;
    WorldToScreen(CameraMatrix, CameraPosition, *LocalScreenPos, width, height);
    Vector3f meme;

    TarkovPlayer* pMyself = Tarkov->GetLocalPlayer();
    if (pMyself == nullptr)
        return;
    TarkovPlayer myself = *pMyself;
    std::string myGroupId = myself.GetPlayerProfile().GetPlayerInfo().GetGroupID().GetString();
    Vector3f myPosition = myself.GetPlayerBody().GetSkeletonRoot().GetLocationMatrixTest();

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
        bool is_friend = groupId == myGroupId;
        if (distance < 150.f && use_aimbot) //&& !is_friend)
        {
            Vector3f aimAngle = CalculateAngle( myself.GetPlayerBody().GetSkeletonRoot().GetLocationMatrixTest(133), headPosition );
            fov = AngleFOV( localView, aimAngle ) / distance;
            if( fov < bestFov){
                bestFov = fov;
                chosenPlayer = Player.Address;
                chosenPlayerAngle = aimAngle;
            }
        }

        WorldToScreen(CameraMatrix, headPosition, *HeadScreenPos, width, height);
        *HeadScreenPos += *LocalScreenPos;

        TarkovESPObject Object;
        strcpy(Object.pName, (Player.IsScav() ? Player.IsPlayerScav() ? "Player Scav" : "Scav" : Player.GetPlayerProfile().GetPlayerInfo().GetPlayerName().GetString()).c_str());
        if (Player.IsScav())
        {
            Object.r = 255 / 255.f;
            Object.g = 255 / 255.f;
            Object.b = 0 / 255.f;
        } else
        {
            if (groupId == myGroupId)
            {
                Object.r = 0 / 255.f;
                Object.g = 255 / 255.f;
                Object.b = 0 / 255.f;
            } else
            {
                Object.r = 255 / 255.f;
                Object.g = 0 / 255.f;
                Object.b = 0 / 255.f;
            }
        }
        Object.x = ScreenPos->x;
        Object.y = ScreenPos->y;
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

    if( !chosenPlayer || bestFov > 180.0f ){
        return;
    }

    GameProcess->Write<Vector2f>( movement.Address + 0x1d8,
                                  Vector2f(chosenPlayerAngle.x, chosenPlayerAngle.y) );
}



void GetTarkovLoot(TarkovGame *Tarkov, TarkovESPArray *a, float width, float height)
{
    Matrix4f CameraMatrix = Tarkov->GetCameraMatrix();
    Vector3f CameraPosition = Tarkov->GetCameraLocation();
    std::vector<TarkovLootItem*> Items = Tarkov->GetLootList();

    // do not clear array as there's players in it
    //clearArray(a);

    TarkovPlayer* pMyself = Tarkov->GetLocalPlayer();
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

        TarkovESPObject Object;
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