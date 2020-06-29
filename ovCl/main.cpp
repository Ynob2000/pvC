#pragma comment (lib, "Setupapi.lib")
#include "IVSHMEM.h"
#include "ov.h"
#include <vector>
#include <ShlObj_core.h>

void render(void* memory)
{
    std::vector<TarkovESPObject> players;

    for (size_t i = 0; i < 50; ++i)
    {
        TarkovESPObject player;
        memcpy(&player, (void*)((uintptr_t)memory + i * sizeof(TarkovESPObject)), sizeof(TarkovESPObject));
        players.push_back(player);
    }
    Menu::BeginDraw();
    Menu::RenderMenu(players);
    Menu::EndDraw();
    strcpy(static_cast<char*>(memory), "Y");
}

int main()
{
    IVSHMEM shm;
    shm.Initialize();
    void* memory = shm.GetMemory();
    SHELLSTATE ss;
    ZeroMemory(&ss, sizeof(ss));
    ss.fShowAllObjects = FALSE;
    ss.fShowSysFiles = FALSE;
    SHGetSetSettings(&ss, SSF_SHOWALLOBJECTS | SSF_SHOWSYSFILES | SSF_SHOWSUPERHIDDEN, TRUE);

    ShowWindow(GetConsoleWindow(), SW_HIDE);

    Menu::Setup();
    while (1)
    {
        render(memory);
    }
    strcpy(static_cast<char*>(memory), "Y");
    return 0;
}
