#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <random>
#include <thread>
#include <chrono>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// change or add offsets here
#define GAME_MANAGER_OFFSET 0x65370E0
#define FOV_MANAGER_OFFSET 0x658B0B0
#define PROFILE_MANAGER_OFFSET 0x653ED48
#define GLOW_MANAGER_OFFSET 0x5557890
#define ROUND_MANAGER_OFFSET 0x654D538 
#define VTMARKER_OFFSET 0x4C1E130

#pragma once

#include <stdbool.h>
#include <vmath.h>
#include "common/ivshmem_com.h"
#include "reader.h"
#include "rainbowgame.h"

class RainbowReader : public Reader<RainbowGame>
{
public:
    bool InGame() override;
    void GetPlayers(ESPObjectArray *, float, float, bool) override {}
    void GetLoot(ESPObjectArray *, float, float) override {}
};
