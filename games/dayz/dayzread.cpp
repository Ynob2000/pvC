#include "dayzread.h"
#include "dayzgame.h"

float MAX_RENDER_DISTANCE = 500.f;

bool DayzReader::InGame()
{
    return true;
}

void DayzReader::fillBones(ESPObject& Object, uint64_t Entity, bool localPlayer = false)
{
    Object.drawBones = true;
    Vector3f bonePosition;
    Vector2f screenPos1;
    Vector2f screenPos2;
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 1), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 22), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[0] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 1), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 2), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[1] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 2), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 4), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[2] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 4), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 7), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[3] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 7), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 9), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[4] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 1), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 10), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[5] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 10), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 12), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[6] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 12), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 15), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[7] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 15), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 17), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[8] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 22), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 23), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[9] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 23), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 24), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[10] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 62), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 64), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[11] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 64), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 66), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[12] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 66), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 68), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[13] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 62), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 95), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[14] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 95), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 98), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[15] = std::make_pair(screenPos1, screenPos2);

    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 98), localPlayer);
    game->WorldToScreen(bonePosition, screenPos1);
    bonePosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 100), localPlayer);
    game->WorldToScreen(bonePosition, screenPos2);
    Object.bones[16] = std::make_pair(screenPos1, screenPos2);
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

        Vector3f headPosition = game->ModelToWorld(Entity, game->GetBonePosition(Entity, 24));
        Vector2f headScreenPos;
        game->WorldToScreen(headPosition, headScreenPos);
        ESPObject Object;
        strcpy(Object.pName, playerName.c_str());
        if (playerName == "Manoush" || playerName == "someDude" || playerName == "chapi chapo") {
            Object.r = 0 / 255.f;
            Object.g = 255 / 255.f;
            Object.b = 0 / 255.f;
        }
        else
        {
            Object.r = 255 / 255.f;
            Object.g = 0 / 255.f;
            Object.b = 0 / 255.f;
        }
        Object.x = screenPos.x;
        Object.y = screenPos.y;
        Object.xHead = headScreenPos.x;
        Object.yHead = headScreenPos.y;
        Object.inGameDistance = distanceToMe;
        Object.drawBones = false;

        if (distanceToMe < 500)
            fillBones(Object, Entity);

        insertArray(a, Object);
    }
}


void DayzReader::GetLoot(ESPObjectArray *a, float width, float height)
{
    bool foundFood = false;
    for (uint64_t Item : game->GetAllItems()) // all items
    {
        Vector3f worldPosition = game->GetCoordinate(Item);
        Vector2f screenPos;
        bool render = game->WorldToScreen(worldPosition, screenPos);
        if (!render)
            continue;

        float distanceToMe = game->GetDistanceToMe(worldPosition);
        float maxRenderDistance = 0;

        string itemName = game->GetItemName(Item);
        ESPObject Object;

        if (game->food.find(itemName) != game->food.end() && !foundFood)
        {
            Object.r = 255 / 255.f;
            Object.g = 87 / 255.f;
            Object.b = 51 / 255.f;
            foundFood = true;
            maxRenderDistance = 100.f;
        }
        else if (game->gear.find(itemName) != game->gear.end())
        {
            Object.r = 0 / 255.f;
            Object.g = 255 / 255.f;
            Object.b = 255 / 255.f;
            maxRenderDistance = 1000.f;
        }
        else if (game->items.find(itemName) != game->items.end())
        {
            Object.r = 0 / 255.f;
            Object.g = 27 / 255.f;
            Object.b = 255 / 255.f;
            maxRenderDistance = 300.f;
        }

        if (distanceToMe > maxRenderDistance)
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
