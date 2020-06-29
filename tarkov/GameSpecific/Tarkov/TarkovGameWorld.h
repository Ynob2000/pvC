#pragma once
#include "GameTypes/Unity/GameWorld.h"
#include "TarkovPlayer.h"
#include "TarkovLootItem.h"
#include "TarkovExfilPoint.h"

class TarkovGameWorld : public GameWorld
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    bool IsValid;

    TarkovGameWorld()
    {
        Address = 0x0;
        IsValid = false;
    }

    TarkovGameWorld(WinProcess *GameProc, GameObject Object)
    {
        GameProcess = GameProc;
        Address = ReadPtrChain(GameProc, Object.GetComponent(), {0x18, 0x28});
        IsValid = true;
    }

    int32_t GetPlayerCount()
    {
        uint64_t m_pPlayerList = GameProcess->Read<uint64_t>(Address + 0x78);

        return GameProcess->Read<int32_t>(m_pPlayerList + 0x18);
    }

    std::vector<TarkovPlayer*> GetAllPlayers()
    {
        uint64_t m_pPlayerList = GameProcess->Read<uint64_t>(Address + 0x78);

        int32_t m_pPlayerListSize = GameProcess->Read<int32_t>(m_pPlayerList + 0x18);
        std::vector<uint64_t> PlayerList(m_pPlayerListSize);

        uint64_t m_pPlayerListObject = GameProcess->Read<uint64_t>(m_pPlayerList + 0x10);
        GameProcess->Read(m_pPlayerListObject + 0x20, PlayerList.data(), m_pPlayerListSize * 0x8);

        std::vector<TarkovPlayer*> out;
        for (uint64_t PlayerPtr : PlayerList) {
            out.push_back(new TarkovPlayer(GameProcess, PlayerPtr));
        }
        return out;
    }

    TarkovPlayer* GetLocalPlayer(std::vector<TarkovPlayer*>& PlayerList)
    {
        for (auto p : PlayerList){
            if (p->IsLocalPlayer()){
                return p;
            }
        }
        return NULL;
    }

    std::vector<TarkovLootItem*> GetAllLoot()
    {
        uint64_t m_lLootList = GameProcess->Read<uint64_t>(Address + 0x58);

        int32_t m_lLootListSize = GameProcess->Read<int32_t>(m_lLootList + 0x18);
        std::vector<uint64_t> LootList(m_lLootListSize);

        uint64_t m_lLootListObject = GameProcess->Read<uint64_t>(m_lLootList + 0x10);
        GameProcess->Read(m_lLootListObject + 0x20, LootList.data(), m_lLootListSize * 0x8);

        std::vector<TarkovLootItem*> out;
        for (uint64_t ItemPtr : LootList) {
            out.push_back(new TarkovLootItem(GameProcess, ItemPtr));
        }
        return out;
    }

    std::vector<uint64_t> GetExfilArray()
    {
        uint64_t m_eExfilController = GameProcess->Read<uint64_t>(GameProcess->Read<uint64_t>(Address + 0x18) + 0x20);

        int64_t m_eExfilListSize = GameProcess->Read<int64_t>(m_eExfilController + 0x18);
        std::vector<uint64_t> ExfilList(m_eExfilListSize);

        GameProcess->Read(m_eExfilController + 0x20, ExfilList.data(), m_eExfilListSize * 0x8);

        /*
        for (int i = 0; i < m_eExfilListSize; i++)
        {
            TarkovExfilPoint m_eExfilPoint = TarkovExfilPoint(GameProcess, GameProcess->Read<uint64_t>(m_eExfilController + 0x20 + (i * 0x8)));
            ExfilList.push_back(m_eExfilPoint);
        }
        */

        return ExfilList;
    }
};