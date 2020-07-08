#pragma once
#include "Includes.h"
#include "TarkovMovementContext.h"
#include "TarkovHandsController.h"

class TarkovProceduralWeaponAnimation
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    std::string ID;

    TarkovProceduralWeaponAnimation(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    void noRecoil()
    {
        // Write ShouldMoveWeaponCloser
        GameProcess->Write<bool>(Address + 0x14e, false);
        // Write Pitch
        GameProcess->Write<float>(Address + 0x2d0, 0.f);
        // Write SwayFalloff
        GameProcess->Write<float>(Address + 0x200, 0.f);

        // Shooting object
        uint64_t shootingg = GameProcess->Read<uint64_t>(Address + 0x48);
        // Write intensity
        GameProcess->Write<float>(shootingg + 0x68, 0.f);
        // Write stiffness
        GameProcess->Write<float>(shootingg + 0x64, 0.f);
        
        // Breath object
        uint64_t breath = GameProcess->Read<uint64_t>(Address + 0x28);
        // Write intensity
        GameProcess->Write<float>(breath + 0xa4, 0.f);
        // Write overweight
        GameProcess->Write<float>(breath + 0xe8, 0.f);

        // Walk object
        uint64_t walk = GameProcess->Read<uint64_t>(Address + 0x30);
        // Write intensity
        GameProcess->Write<float>(walk + 0x44, 0.f);

        // MotionEffector object
        uint64_t motionEffector = GameProcess->Read<uint64_t>(Address + 0x38);
        // Write intensity
        GameProcess->Write<float>(motionEffector + 0xd0, 0.f);
        // Fetch sway factors
        uint64_t swayFactors = GameProcess->Read<uint64_t>(motionEffector + 0xa4);
        // Overwrite x, y, z
        GameProcess->Write<float>(swayFactors + 0x10, 0.f); // x
        GameProcess->Write<float>(swayFactors + 0x14, 0.f); // y
        GameProcess->Write<float>(swayFactors + 0x18, 0.f); // z

        // ForceEffector object
        uint64_t forceEffector = GameProcess->Read<uint64_t>(Address + 0x40);
        // Write intensity
        GameProcess->Write<float>(forceEffector + 0x30, 0.f);
    }
};
