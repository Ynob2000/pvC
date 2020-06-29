//#include "Includes.h"
#include "tarkov/GameSpecific/Tarkov/Tarkov.h"
#include "tarkovread.h"
extern "C" {
#include "common/ivshmem.h"
#include "common/debug.h"
}
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

struct test{
    int a;
};

bool waitOk(void* mem)
{
    return *static_cast<char*>(mem) == 'Y';
}

void send_data(IVSHMEM* shm, TarkovESPArray* data)
{
    void* memory = shm->mem;
    while (!waitOk(memory))
    {

    }
    size_t size = data->size;
    TarkovESPObject * array = data->array;
    for (size_t t = 0; t < size; ++t) {
        memcpy((void*)((uintptr_t)memory + t * sizeof(TarkovESPObject)),
                array + t,
                sizeof(TarkovESPObject));
    }
    int a = 1;
}

bool prepare_ivshmem(IVSHMEM* shm)
{
    ivshmemOptionsInit();
    if (!ivshmemOpen(shm))
    {
        DEBUG_ERROR("Failed to map memory");
        return false;
    }
    TarkovESPArray * a = new TarkovESPArray;
    initArray(a, 5);
    for (int i = 0; i < 5; ++i)
    {
        TarkovESPObject Object;
        Object.x = i;

        insertArray(a, Object);
    }
    send_data(shm, a);
    return true;
}


void main_loop(TarkovGame* tarkov, IVSHMEM* shm)
{
    std::time_t ticker = std::time(nullptr);
    TarkovESPArray playerarray;
    while (true)
    {
        if (std::time(nullptr) - ticker > 1) {
            bool ret = tarkov->Tick();
            if (!ret)
                break;
        }
        GetTarkovPlayers(tarkov, &playerarray, 1920, 1080);
        ticker = std::time(nullptr);
    }
}

int main()
{
    IVSHMEM shm;
    if (!prepare_ivshmem(&shm))
    {
        DEBUG_ERROR("Failed to prepare IVSHMEM");
        return 1;
    }
    TarkovGame *Tarkov;
    while (!GetTarkovGame(&Tarkov, "EscapeFromTarkov.exe", "UnityPlayer.dll"))
    {
        DEBUG_ERROR("Failed to find Tarkov");
        std::this_thread::sleep_for(2s);
    }
    while (!TarkovInGame(Tarkov))
    {
        DEBUG_INFO("Waiting for game to start");
        std::this_thread::sleep_for(2s);
    }
    main_loop(Tarkov, &shm);
}
