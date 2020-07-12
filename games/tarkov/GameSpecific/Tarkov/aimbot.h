#include "Includes.h"
#include "TarkovPlayer.h"
#include <vmath.h>

#include <csignal>

#define RAD2DEG 57.295779513f

Vector3f CalculateAngle(const Vector3f &local, const Vector3f &enemy) {
    Vector3f delta = local - enemy;
    float len = delta.length();

    Vector3f ret;
    ret.y = asinf(delta.y / len ) * RAD2DEG;
    ret.x = -atan2f(delta.x, -delta.z) * RAD2DEG;

    return ret;
}

inline float AngleFOV(const Vector3f &viewAngle, const Vector3f &aimAngle) {
    Vector3f delta = viewAngle - aimAngle;
    if (delta.x < -180.f)
        delta.x += 360.f;
    if (delta.x > 180.f)
        delta.x -= 360.f;
    return fabsf(delta.length());
}