#pragma once

#include <stdbool.h>
#include <vmath.h>

#if __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* todo sound
* sudo ./looking-glass-client -s -k -g EGLTarkov
*/

typedef struct TarkovGame TarkovGame;

typedef struct SDL_Window SDL_Window;

typedef struct
{
    char pName[100];
    float x, y;
    float xHead, yHead;
    float distance;
    float inGameDistance;
    float r, g, b;
    bool drawBones;
    std::pair<Vector2f, Vector2f> bones[10];
}TarkovESPObject;

typedef struct
{
    TarkovESPObject *array;
    int used;
    int size;
} TarkovESPArray;

EXTERNC bool GetTarkovGame(TarkovGame **Tarkov, const char *ProcessName, const char *ModuleName);
EXTERNC bool InitImGui(SDL_Window* window, void* context);
EXTERNC bool runImGui(SDL_Window* window);
EXTERNC void DestroyTarkovGame(TarkovGame *Tarkov);
EXTERNC bool TickTarkov(TarkovGame *Tarkov);

EXTERNC bool TarkovInGame(TarkovGame *Tarkov);

EXTERNC bool initArray(TarkovESPArray *a, int initialSize);
EXTERNC void insertArray(TarkovESPArray *a, TarkovESPObject element);
EXTERNC void clearArray(TarkovESPArray *a);
EXTERNC void freeArray(TarkovESPArray *a);

EXTERNC void GetTarkovPlayers(TarkovGame *Tarkov, TarkovESPArray *a, float width, float height, bool use_aimbot);
EXTERNC void GetTarkovLoot(TarkovGame *Tarkov, TarkovESPArray *a, float width, float height);