//
// Created by nicolas on 6/28/20.
//

#include "Includes.h"

uint64_t ReadPtrChain(WinProcess *GameProc, uint64_t Base, std::vector<uint64_t> Chain)
{
    uint64_t result = GameProc->Read<uint64_t>(Base + Chain[0]);
    for (uint64_t i = 1; i < Chain.size(); i++)
    {
        result = GameProc->Read<uint64_t>(result + Chain[i]);
    }
    return result;
}

bool WorldToScreen(Matrix4f CameraMatrix, Vector3f WorldPosition, Vector2f &ScreenPosition, float width, float height)
{
    CameraMatrix = CameraMatrix.transpose();

    const Vector3f translation = {CameraMatrix.at(3, 0), CameraMatrix.at(3, 1), CameraMatrix.at(3, 2)};
    const Vector3f up = {CameraMatrix.at(1, 0), CameraMatrix.at(1, 1), CameraMatrix.at(1, 2)};
    const Vector3f right = {CameraMatrix.at(0, 0), CameraMatrix.at(0, 1), CameraMatrix.at(0, 2)};

    const auto w = translation.dotProduct(WorldPosition) + CameraMatrix.at(3,3);

    if (w < 0.1f)
        return false;

    const auto x = right.dotProduct(WorldPosition) + CameraMatrix.at(0, 3);
    const auto y = up.dotProduct(WorldPosition) + CameraMatrix.at(1, 3);

    ScreenPosition.x = (width) * (1.f + x / w) - (width);
    ScreenPosition.y = -((height) * (1.f - y / w) - (height));

    return true;
}

float clamp(float x, float min, float max)
{
    if (x < min)
        x = min;
    else if (x > max)
        x = max;
    return x;
}

float CalcInFov(const Vector3f& position, Matrix4f& cameraMatrix, const Vector3f& vector)
{
    Vector3f diff = position - vector;
    diff.normalize();
    Matrix4f t = cameraMatrix.transpose();
    Vector3f forward = { t.at(2, 0), t.at(2, 1), t.at(2, 2) };
    return acos(clamp(forward.dotProduct(diff), -1.f, 1.f)) * 57.29578f;
}

Vector3f toEuler(const Vector3f& v, float angle) {
    float x = v.x;
    float y = v.y;
    float z = v.z;
    double s = sin(angle);
    double c = cos(angle);
    double t = 1 - c;
    float heading, attitude, bank;

    if ((x * y * t + z * s) > 0.998) { // north pole singularity detected
        heading = 2 * atan2(x * sin(angle / 2), cos(angle / 2));
        attitude = acos(0);
        bank = 0;
        return Vector3f(heading, attitude, bank);
    }
    if ((x * y * t + z * s) < -0.998) { // south pole singularity detected
        heading = -2 * atan2(x * sin(angle / 2), cos(angle / 2));
        attitude = -acos(0);
        bank = 0;
        return Vector3f(heading, attitude, bank);
    }
    heading = atan2(y * s - x * z * t, 1 - (y * y + z * z) * t);
    attitude = asin(x * y * t + z * s);
    bank = atan2(x * s - y * z * t, 1 - (x * x + z * z) * t);
    return Vector3f(heading, attitude, bank);
}