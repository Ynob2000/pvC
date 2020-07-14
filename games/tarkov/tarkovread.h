#pragma once

#include <stdbool.h>
#include <vmath.h>
#include "common/ivshmem_com.h"
#include "reader.h"
#include "GameSpecific/Tarkov/Tarkov.h"

class TarkovReader : public Reader<TarkovGame>
{
public:

    struct color {float r,g,b;};
    int colorCount = 0;
    std::map<std::string, color> teamColors;
    std::vector<color> allColors =  {color{255, 87, 51},
                                     color{0, 255, 254},
                                     color{0, 27, 255},
                                     color{145, 0, 0},
                                     color{78, 255, 169},
                                     color{110, 78, 255},};

    color GetNewColor()
    {
        color out = allColors[colorCount];
        ++colorCount;
        return out;
    }

    static void fillBones(const Matrix4f&, TarkovPlayerBones&, TarkovSkeletonRoot&, float, float,const Vector2f&, ESPObject&);

    bool InGame() override;
    void GetPlayers(ESPObjectArray *, float, float, bool) override;
    void GetLoot(ESPObjectArray *, float, float) override;
};