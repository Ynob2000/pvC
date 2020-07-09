#pragma once
#include "Includes.h"

class Game
{
public:
    WinContext *ProcessContext;
    uint64_t ModuleBase;
    WinProcess *GameProcess;

    ~Game()
    {
        delete ProcessContext;
    }

    bool Tick()
    {
        return true;
    }
};