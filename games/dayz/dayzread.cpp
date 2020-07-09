#include "dayzread.h"
#include "dayzgame.h"

float MAX_RENDER_DISTANCE = 500.f;

bool DayzReader::InGame()
{
    return true;
}

void DayzReader::GetPlayers(ESPObjectArray *a, float width, float height, bool use_aimbot)
{
    for (uint64_t Entity : game->GetAllEntityes()) // all players
    {
        Vector3 worldPosition = game->GetCoordinate(Entity);
        Vector3 screenPos = game->WorldToScreen(worldPosition);

        float distanceToMe = game->GetDistanceToMe(worldPosition);
        string playerName = game->GetPlayerName(Entity);

        printf("PlayerName: %s                \n", playerName.c_str());
        printf("Type: %s                      \n", game->GetTypeName(Entity).c_str());
        printf("Model: %s                     \n", game->GetModelName(Entity).c_str());
        printf("Item in Hands: %s             \n", game->GetItemInHands(Entity).c_str());
        printf("Distance: %.0fm               \n", distanceToMe);
        printf("Position: %.1f : %.1f : %.1f  \n", worldPosition.x, worldPosition.y, worldPosition.z);
        printf("ScreenPosition: %.1f : %.1f    \n", screenPos.x, screenPos.y);
        ESPObject Object;
        strcpy(Object.pName, playerName.c_str());
        Object.r = 255 / 255.f;
        Object.g = 0 / 255.f;
        Object.b = 0 / 255.f;
        Object.x = screenPos.x;
        Object.y = screenPos.y;
        Object.xHead = screenPos.x;
        Object.yHead = screenPos.y;
        Object.inGameDistance = distanceToMe;
        Object.drawBones = false;

        insertArray(a, Object);
    }
}

void DayzReader::GetLoot(ESPObjectArray *a, float width, float height)
{

}
