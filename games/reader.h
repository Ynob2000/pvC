#pragma once
#include "game.h"
#include "common/ivshmem_com.h"

template<typename T>
class Reader
{
public:
    T* game;
    void DestroyGame(){
        delete this->game;
    };

    bool Tick(){
        if (this->game == nullptr)
            return false;

        return this->game->Tick();
    };

    virtual bool InGame() = 0;

    virtual void GetPlayers(ESPObjectArray *, float, float, bool) = 0;
    virtual void GetLoot(ESPObjectArray *, float, float) = 0;

    bool GetGame(const char *ProcessName, const char *ModuleName){
        FILE *out = stdout;
        pid_t pid;

#if (LMODE() == MODE_EXTERNAL())
        FILE *pipe = popen("pidof qemu-system-x86_64", "r");
        fscanf(pipe, "%d", &pid);
        pclose(pipe);
#else
        out = fopen("/tmp/testr.txt", "w");
    pid = getpid();
#endif

        try
        {
            WinContext *ctx = new WinContext(pid);
            ctx->processList.Refresh();

            fprintf(out, "\nInitializing Game Offsets\n");
            bool ProcessFound = false;

            for (auto &i : ctx->processList)
            {
                if (!strcasecmp(ProcessName, i.proc.name))
                {
                    fprintf(out, "Game:\tLooping process %lx:%s\n", i.proc.dirBase, i.proc.name);
                    ProcessFound = true;

                    for (auto &o : i.modules)
                    {
                        fprintf(out, "\t%.8lx\t%.8lx\t%lx\t%s\n", o.info.baseAddress, o.info.entryPoint, o.info.sizeOfModule, o.info.name);
                        if (!strcmp(ModuleName, o.info.name))
                        {
                            this->game = new T(ctx, &i, o.info.baseAddress);
                            return true;
                        }
                    }
                }
            }

            if (!ProcessFound)
                fprintf(out, "Error finding process: %s\n", ProcessName);

            fprintf(out, "Error finding module: %s\n", ModuleName);
        }
        catch (VMException &e)
        {
            fprintf(out, "Initialization error: %d\n", e.value);
        }

        return false;
    };
};

