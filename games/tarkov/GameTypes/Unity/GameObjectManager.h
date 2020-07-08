#pragma once
#include "Includes.h"
#include "BaseObject.h"
#include "tarkov/GameSpecific/Tarkov/TarkovGameWorld.h"

class GameObjectManager
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    GameObjectManager(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    GameObjectManager()
    {
        Address = 0x0;
    }

    TarkovGameWorld FindGameObjectActive(const char* Object)
    {
        int32_t LoopDetector = 0;
        for (BaseObject ActiveObject = GetFirstActiveObject(); ActiveObject.Address != GetLastActiveObject().Address; ActiveObject = ActiveObject.GetNextBaseObject())
        {
            if (++LoopDetector > 10000) /* really should be the first couple items, if its any more its probably not found and looping */
            {
                break;
            }
            std::string ActiveObjectName = ActiveObject.GetGameObject().GetGameObjectName();
            if (ActiveObjectName.find(Object) != std::string::npos)
            {
                TarkovGameWorld out = TarkovGameWorld(GameProcess, ActiveObject.GetGameObject());
                if (out.GetPlayerCount() == 0)
                    continue;
                printf("%s found.\n", Object);
                return out;
            }
        }
        return TarkovGameWorld();
    }

    template<class Type>
    Type FindGameObjectTagged(const char* Object)
    {
        int32_t LoopDetector = 0;
        for (BaseObject ActiveObject = GetFirstTaggedObject(); ; ActiveObject = ActiveObject.GetNextBaseObject())
        {
            if (++LoopDetector > 50000) /* really should be the first couple items, if its any more its probably not found and looping */
            {
                break;
            }
            std::string ActiveObjectName = ActiveObject.GetGameObject().GetGameObjectName();
            if (ActiveObjectName.find(Object) != std::string::npos)
            {
                printf("%s found.\n", Object);
                return Type(GameProcess, ActiveObject.GetGameObject());
            }
            if (ActiveObject.Address == GetLastTaggedObject().Address)
                break;
        }

        return Type();
    }

    BaseObject GetLastTaggedObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address));
    }

    BaseObject GetFirstTaggedObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x8));
    }

    BaseObject GetLastActiveObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10));
    }

    BaseObject GetFirstActiveObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x18));
    }
};