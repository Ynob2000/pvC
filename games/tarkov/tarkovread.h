#pragma once

#include <stdbool.h>
#include <vmath.h>
#include "common/ivshmem_com.h"
#include "reader.h"
#include "GameSpecific/Tarkov/Tarkov.h"

class TarkovReader : public Reader<TarkovGame>
{
public:
    static void fillBones(const Matrix4f&, TarkovPlayerBones&, TarkovSkeletonRoot&, float, float, Vector2f*, ESPObject&);

    bool InGame() override;
    void GetPlayers(ESPObjectArray *, float, float, bool) override;
    void GetLoot(ESPObjectArray *, float, float) override;
};