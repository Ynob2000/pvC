#pragma once
#include "game.h"
#include "common/ivshmem_com.h"

class Reader
{
public:
    virtual void DestroyGame(Game *) = 0;
    virtual bool Tick(Game *) = 0;

    virtual bool InGame(Game *) = 0;

    virtual void GetPlayers(Game *, ESPObjectArray *, float, float, bool) = 0;
    virtual void GetLoot(Game *, ESPObjectArray *, float, float) = 0;

    virtual bool GetGame(Game**, const char *, const char *) = 0;
};
