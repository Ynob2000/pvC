#pragma once
#include "Includes.h"

class Game
{
private:
    WinContext *ProcessContext;
    WinProcess *GameProcess;
    uint64_t ModuleBase;

public:
    ~Game()
    {
        delete ProcessContext;
    }

    bool Tick()
    {
        return true;
    }
};