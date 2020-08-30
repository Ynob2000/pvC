#pragma once
#include "game.h"
#include "vmath.h"
#include <set>

class RainbowGame : public Game
{
public:
    RainbowGame(WinContext *ctx, WinProcess *InProcess, uint64_t InModuleBase)
    {
        ProcessContext = ctx;
        GameProcess = InProcess;
        ModuleBase = InModuleBase;
    }
};
