//
// Created by nicolas on 6/28/20.
//

#include <xmmintrin.h>
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

struct TransformAccessReadOnly {
    uint64_t pTransformData;
    int index;
};

struct TransformData {
    uint64_t pTransformArray;
    uint64_t pTransformIndices;
};

struct Matrix34 {
    Vector4f vec0;
    Vector4f vec1;
    Vector4f vec2;
};

Vector3f get_transform_position(WinProcess* GameProcess, uint64_t transform) {
    auto transform_internal = GameProcess->Read<uint64_t>(transform + 0x10);
    if (!transform_internal)
        return Vector3f();

    __m128 result;

    const __m128 mulVec0 = {-2.000, 2.000, -2.000, 0.000};
    const __m128 mulVec1 = {2.000, -2.000, -2.000, 0.000};
    const __m128 mulVec2 = {-2.000, -2.000, 2.000, 0.000};

    auto pTransformAccessReadOnly = GameProcess->Read<TransformAccessReadOnly>(transform_internal + 0x38);

    auto transformData = GameProcess->Read<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

    size_t sizeMatriciesBuf = sizeof(Matrix34) * pTransformAccessReadOnly.index + sizeof(Matrix34);
    size_t sizeIndicesBuf = sizeof(int) * pTransformAccessReadOnly.index + sizeof(int);

    // Allocate memory for storing large amounts of data (matricies and indicies)
    void* pMatriciesBuf = malloc(sizeMatriciesBuf);
    void* pIndicesBuf = malloc(sizeIndicesBuf);

    if (pMatriciesBuf && pIndicesBuf) {
        // Read Matricies array into the buffer
        char *matricies_buf_buf = new char[sizeMatriciesBuf];
        GameProcess->ReadRaw(matricies_buf_buf, transformData.pTransformArray, sizeMatriciesBuf);
        memcpy(pMatriciesBuf, matricies_buf_buf, sizeMatriciesBuf);
        delete[] matricies_buf_buf;

        // Read Indices array into the buffer
        char *indices_buf_buf = new char[sizeIndicesBuf];
        GameProcess->ReadRaw(indices_buf_buf, transformData.pTransformIndices, sizeIndicesBuf);
        memcpy(pIndicesBuf, indices_buf_buf, sizeIndicesBuf);
        delete[] indices_buf_buf;

        result = *(__m128 * )((uint64_t) pMatriciesBuf + 0x30 * pTransformAccessReadOnly.index);
        int transformIndex = *(int *) ((uint64_t) pIndicesBuf + 0x4 * pTransformAccessReadOnly.index);
        int transformIndexInit = transformIndex;
        for (int i = 0; transformIndex >= 0 && i < 30 && fabs(transformIndex - transformIndexInit) < 1000000; i++)
        {
            Matrix34 matrix34 = *(Matrix34*)((uint64_t)pMatriciesBuf + 0x30 * transformIndex);

            __m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) &matrix34.vec1), 0x00));	// xxxx
            __m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) &matrix34.vec1), 0x55));	// yyyy
            __m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) &matrix34.vec1), 0x8E));	// zwxy
            __m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) &matrix34.vec1), 0xDB));	// wzyw
            __m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) &matrix34.vec1), 0xAA));	// zzzz
            __m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) &matrix34.vec1), 0x71));	// yxwy
            __m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

            result = _mm_add_ps(
                    _mm_add_ps(
                            _mm_add_ps(
                                    _mm_mul_ps(
                                            _mm_sub_ps(
                                                    _mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
                                                    _mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
                                            _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
                                    _mm_mul_ps(
                                            _mm_sub_ps(
                                                    _mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
                                                    _mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
                                            _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
                            _mm_add_ps(
                                    _mm_mul_ps(
                                            _mm_sub_ps(
                                                    _mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
                                                    _mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
                                            _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
                                    tmp7)), _mm_load_ps(&matrix34.vec0.x));

            transformIndex = *(int*)((uint64_t)pIndicesBuf + 0x4 * transformIndex);
        }

        free(pMatriciesBuf);
        free(pIndicesBuf);
    }

    return Vector3f(result[0], result[1], result[2]);
}