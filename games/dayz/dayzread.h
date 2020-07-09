#pragma once

#include <vmath.h>
#include "common/ivshmem_com.h"
#include "reader.h"
#include "dayzgame.h"


class DayzReader : public Reader<DayzGame>
{
public:

    bool InGame() override;
    void GetPlayers(ESPObjectArray *, float, float, bool) override;
    void GetLoot(ESPObjectArray *, float, float) override;
};