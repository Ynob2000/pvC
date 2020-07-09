#include "common/ivshmem_com.h"

#if defined(TARKOV)
    #include "tarkov/tarkovread.h"
    #define EXECUTABLE "EscapeFromTarkov.exe"
    #define MODULE_BASE "UnityPlayer.dll"
    #define READER TarkovReader
#elif defined(DAYZ)
    #include "dayz/dayzread.h"
    #define EXECUTABLE "DayZ_x64.exe"
    #define MODULE_BASE "DayZ_x64.exe"
    #define READER DayzReader
#endif

extern "C" {
#include "common/ivshmem.h"
#include "common/debug.h"
}
#include <chrono>
#include <thread>

using namespace std::chrono_literals;


void send_data(IVSHMEM* shm, ESPObjectArray* data)
{
    void* memory = shm->mem;
    size_t size = data->size;
    ESPObject * array = data->array;
    for (size_t t = 0; t < size; ++t) {
        memcpy((void*)((uintptr_t)memory + t * sizeof(ESPObject)),
                array + t,
                sizeof(ESPObject));
    }
}

bool use_aimbot(IVSHMEM* shm)
{
    void* memory = shm->mem;
    return *(char*)((uintptr_t)memory + 0x123456) == 'Y';
}

bool prepare_ivshmem(IVSHMEM* shm)
{
    ivshmemOptionsInit();
    if (!ivshmemOpen(shm))
    {
        DEBUG_ERROR("Failed to map memory");
        return false;
    }
    return true;
}

void main_loop(READER* reader, IVSHMEM* shm) {
    ESPObjectArray dataarray;
    initArray(&dataarray, 100);
    while (true)
    {
        bool ret = reader->Tick();
        if (!ret)
            break;

        reader->GetPlayers(&dataarray, 1920, 1080, use_aimbot(shm));
        reader->GetLoot(&dataarray, 1920, 1080);
        send_data(shm, &dataarray);
    }
}

int main()
{
    IVSHMEM shm{};
    if (!prepare_ivshmem(&shm))
    {
        DEBUG_ERROR("Failed to prepare IVSHMEM");
        return 1;
    }

    READER reader;

    while (!reader.GetGame(EXECUTABLE, MODULE_BASE))
    {
        DEBUG_ERROR("Failed to find game");
        std::this_thread::sleep_for(2s);
    }
    while (!reader.InGame())
    {
        DEBUG_INFO("Waiting for game to start");
        reader.Tick();
        std::this_thread::sleep_for(2s);
    }
    main_loop(&reader, &shm);
}
