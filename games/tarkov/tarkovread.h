#pragma once

#include <stdbool.h>
#include <vmath.h>
#include "common/ivshmem_com.h"
#include "reader.h"
#include "GameSpecific/Tarkov/Tarkov.h"

#if __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* todo sound
* sudo ./looking-glass-client -s -k -g EGLTarkov
*/

typedef struct SDL_Window SDL_Window;


class TarkovReader : public Reader
{
public:
    void fillBones(const Matrix4f&, TarkovPlayerBones&, TarkovSkeletonRoot&, int, int, Vector2f*, ESPObject&);
    bool GetGame(Game**, const char *, const char *) override;
    void DestroyGame(Game *) override;
    bool Tick(Game *) override;

    bool InGame(Game *) override;

    void GetPlayers(Game *, ESPObjectArray *, float, float, bool) override;
    void GetLoot(Game *, ESPObjectArray *, float, float) override;
};