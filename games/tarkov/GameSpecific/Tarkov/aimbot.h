#include "Includes.h"
#include "TarkovPlayer.h"
#include <vmath.h>

#include <csignal>

#define RAD2DEG 57.295779513f

Vector3f CalculateAngle(const Vector3f &local, const Vector3f &enemy) {
    Vector3f delta = local - enemy;
    //delta.y -= 180.0f;
    //delta.NormalizeAngles();

    float len = delta.length();

    Vector3f ret;
    ret.y = asinf(delta.y / len ) * RAD2DEG;
    ret.x = -atan2f(delta.x, -delta.z) * RAD2DEG;

    return ret;
}

inline float AngleFOV(const Vector3f &viewAngle, const Vector3f &aimAngle) {
    Vector3f delta = viewAngle - aimAngle;
    return fabsf(delta.length());
}

void Aim(WinProcess* GameProcess, std::vector<TarkovPlayer*>& playerList, TarkovPlayer& localPlayer)
{
    TarkovMovementContext movement = localPlayer.GetMovementContext();
    TarkovHandsController handsController = localPlayer.GetPlayerHandsController();
    uint64_t fireport = GameProcess->Read<uint64_t>(handsController.Address + 0xd0);

    void* data = malloc(500);
    GameProcess->ReadRaw(data, fireport, 0x500);

    Vector3f localView;
    localView.x = movement.GetViewAngles1().x;
    localView.y = movement.GetViewAngles1().y;

    Vector3f localPlayerHead = localPlayer.GetPlayerBody().GetSkeletonRoot().GetLocationMatrixTest();

    volatile float fov;
    float bestFov = 999.0f;
    uintptr_t chosenPlayer = 0;
    Vector3f chosenPlayerAngle;

    // Get closest player in fov
    for( TarkovPlayer* playerPtr : playerList) {
        TarkovPlayer player = *playerPtr;
        if (player == localPlayer)
            continue;
        TarkovPlayerBones bones = player.GetPlayerBody().GetPlayerBones();
        Vector3f headPos = bones.GetHeadPosition();
        Vector3f aimAngle = CalculateAngle( localPlayerHead, headPos );

        fov = AngleFOV( localView, aimAngle );
        if( fov < bestFov){
            bestFov = fov;
            chosenPlayer = player.Address;
            chosenPlayerAngle = aimAngle;
        }
    }

    if( !chosenPlayer || bestFov > 180.0f ){
        return;
    }

    GameProcess->Write<Vector2f>( movement.Address + 0x1d0,
            Vector2f(chosenPlayerAngle.x, chosenPlayerAngle.y) );
}