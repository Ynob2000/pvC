#pragma comment (lib, "Setupapi.lib")
#include "IVSHMEM.h"
#include "ov.h"
#include <vector>
#include <ShlObj_core.h>
// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include <thread>
using namespace std::chrono_literals;
void render(void* memory)
{
    std::vector<data> ps;

    for (size_t i = 0; i < 100; ++i)
    {
        data d;
        memcpy(&d, (void*)((uintptr_t)memory + i * sizeof(data)), sizeof(data));
        ps.push_back(d);
    }

    Menu::BeginDraw();
    Menu::RenderMenu(ps);
    Menu::EndDraw();
}

HWND FindTopWindow(DWORD pid)

{
    std::pair<HWND, DWORD> params = { 0, pid };
    // Enumerate the windows using a lambda to process each window
    BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
        {
            auto pParams = (std::pair<HWND, DWORD>*)(lParam);
            DWORD processId;
            if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams->second)
            {
                // Stop enumerating
                SetLastError(-1);
                pParams->first = hwnd;
                return FALSE;
            }
            // Continue enumerating
            return TRUE;
        }, (LPARAM)&params);

    if (!bResult && GetLastError() == -1 && params.first)
    {
        return params.first;
    }

    return 0;
}

DWORD WINAPI trt(LPVOID lpParam)
{
    IVSHMEM shm;
    while (!shm.Initialize())
    {
    }
    void* memory = shm.GetMemory();
    SHELLSTATE ss;
    ZeroMemory(&ss, sizeof(ss));
    ss.fShowAllObjects = FALSE;
    ss.fShowSysFiles = FALSE;
    SHGetSetSettings(&ss, SSF_SHOWALLOBJECTS | SSF_SHOWSYSFILES | SSF_SHOWSUPERHIDDEN, TRUE);

    ShowWindow(GetConsoleWindow(), SW_HIDE);

    DWORD pid = GetCurrentProcessId();
    HWND hwnd = FindTopWindow(pid);
    Menu::Setup(&hwnd);
    while (1)
    {
        if (GetKeyState(VK_XBUTTON1) & 0x8000) 
        {
            memcpy((void*)((uintptr_t)memory + 0x123456), "Y", 1);
        }
        else
        {
            memcpy((void*)((uintptr_t)memory + 0x123456), "N", 1);
        }
        render(memory);
    }
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, &trt, nullptr, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}