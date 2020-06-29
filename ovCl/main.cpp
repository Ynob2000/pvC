#pragma comment (lib, "Setupapi.lib")
#include "IVSHMEM.h"
#include <vector>

typedef struct
{
    char pName[100];
    float x, y;
    float distance;
    float inGameDistance;
    bool render;
    bool isScav;
} TarkovESPObject;

typedef struct
{
    TarkovESPObject* array;
    int used;
    int size;
} TarkovESPArray;

void render(void* memory)
{
    std::vector<TarkovESPObject> players;

    for (size_t i = 0; i < 50; ++i)
    {
        TarkovESPObject player;
        memcpy(&player, (void*)((uintptr_t)memory + i * sizeof(TarkovESPObject)), sizeof(TarkovESPObject));
        players.push_back(player);
    }
    strcpy(static_cast<char*>(memory), "Y");
}

int main()
{
    IVSHMEM shm;
    shm.Initialize();
    void* memory = shm.GetMemory();
    while (1)
    {
        render(memory);
    }
    strcpy(static_cast<char*>(memory), "Y");
    return 0;
}
