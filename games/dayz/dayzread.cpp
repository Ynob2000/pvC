#include "dayzread.h"
#include "dayzgame.h"

float MAX_RENDER_DISTANCE = 500.f;

bool DayzReader::InGame()
{
    return true;
}

void DayzReader::fillBones(ESPObject& Object, uint64_t Entity)
{
    Object.drawBones = true;
    Vector3f playerPosition = game->GetCoordinate(Entity);
    Vector3f playerOrientation = game->GetOrientation(Entity);
    Vector3f bonePosition;
    Vector2f screenPos1;
    Vector2f screenPos2;
    bonePosition = playerPosition + game->GetBonePosition(Entity, 1);
    game->WorldToScreen(playerPosition, screenPos1);
    bonePosition = playerPosition + game->GetBonePosition(Entity, 6);
    game->WorldToScreen(playerPosition + playerOrientation * 5.f, screenPos2);
    Object.bones[0] = std::make_pair(screenPos1, screenPos2);
//    bonePosition = playerPosition + game->GetBonePosition(Entity, 1);
//    game->WorldToScreen(bonePosition, screenPos1);
//    bonePosition = playerPosition + game->GetBonePosition(Entity, 6);
//    game->WorldToScreen(bonePosition, screenPos2);
//    Object.bones[0] = std::make_pair(screenPos1, screenPos2);
//
//    bonePosition = playerPosition + game->GetBonePosition(Entity, 14);
//    game->WorldToScreen(bonePosition, screenPos1);
//    bonePosition = playerPosition + game->GetBonePosition(Entity, 9);
//    game->WorldToScreen(bonePosition, screenPos2);
//    Object.bones[1] = std::make_pair(screenPos1, screenPos2);
}

void DayzReader::GetPlayers(ESPObjectArray *a, float width, float height, bool use_aimbot)
{
    clearArray(a);
    for (uint64_t Entity : game->GetAllEntities()) // all players
    {
        Vector3f worldPosition = game->GetCoordinate(Entity);
        Vector2f screenPos;
        bool render = game->WorldToScreen(worldPosition, screenPos);
        if (!render)
            continue;

        float distanceToMe = game->GetDistanceToMe(worldPosition);
        string playerName = game->GetPlayerName(Entity);

        Vector3f orientation = game->GetOrientation(Entity);
        Vector3f headPosition = worldPosition + game->GetBonePosition(Entity, 24) + orientation;
        Vector2f headScreenPos;
        game->WorldToScreen(headPosition, headScreenPos);

//        printf("PlayerName: %s                \n", playerName.c_str());
//        printf("Ptr: %lu                \n", ptr);
//        printf("Type: %s                      \n", game->GetTypeName(Entity).c_str());
//        printf("Model: %s                     \n", game->GetModelName(Entity).c_str());
//        printf("Item in Hands: %s             \n", game->GetItemInHands(Entity).c_str());
//        printf("Distance: %.0fm               \n", distanceToMe);
//        printf("Position: %.1f : %.1f : %.1f  \n", worldPosition.x, worldPosition.y, worldPosition.z);
//        printf("ScreenPosition: %.1f : %.1f    \n", screenPos.x, screenPos.y);
        ESPObject Object;
        strcpy(Object.pName, playerName.c_str());
        Object.r = 255 / 255.f;
        Object.g = 0 / 255.f;
        Object.b = 0 / 255.f;
        Object.x = screenPos.x;
        Object.y = screenPos.y;
        Object.xHead = headScreenPos.x;
        Object.yHead = headScreenPos.y;
        Object.inGameDistance = distanceToMe;
        Object.drawBones = false;

        if (distanceToMe < 200)
            fillBones(Object, Entity);

        insertArray(a, Object);
    }
}


void DayzReader::GetLoot(ESPObjectArray *a, float width, float height)
{
    for (uint64_t Item : game->GetAllItems()) // all items
    {
        Vector3f worldPosition = game->GetCoordinate(Item);
        Vector2f screenPos;
        bool render = game->WorldToScreen(worldPosition, screenPos);
        if (!render)
            continue;

        float distanceToMe = game->GetDistanceToMe(worldPosition);
        if (distanceToMe > 150)
            continue;
        string itemName = game->GetItemName(Item);
        ESPObject Object;
        if (game->food.find(itemName) != game->food.end() && false)
        {
            Object.r = 255 / 255.f;
            Object.g = 87 / 255.f;
            Object.b = 51 / 255.f;
        }
        else if (game->gear.find(itemName) != game->gear.end())
        {
            Object.r = 0 / 255.f;
            Object.g = 255 / 255.f;
            Object.b = 255 / 255.f;
        }
        else if (game->items.find(itemName) != game->items.end())
        {
            Object.r = 0 / 255.f;
            Object.g = 27 / 255.f;
            Object.b = 255 / 255.f;
        }
        else
            continue;

        strcpy(Object.pName, itemName.c_str());
        Object.x = screenPos.x;
        Object.y = screenPos.y;
        Object.xHead = screenPos.x;
        Object.yHead = screenPos.y;
        Object.inGameDistance = distanceToMe;
        Object.drawBones = false;

        insertArray(a, Object);
    }
}
