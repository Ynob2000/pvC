#pragma once

#include <xmmintrin.h>
#include "Includes.h"
#include "TarkovSkeletonRoot.h"

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


class TarkovSkeletonRoot
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovSkeletonRoot(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    uint64_t getBoneEnum()
    {
        return GameProcess->Read<uint64_t>(Address + 0x28);
    }

    Vector3f GetLocationMatrixTest(int bone_id = 0)
    {
        uint64_t m_BoneEnum = getBoneEnum();
        uint64_t m_TransformArray = GameProcess->Read<uint64_t>(m_BoneEnum + 0x10);
        uint64_t m_TransformA = GameProcess->Read<uint64_t>(m_TransformArray + 0x20 + (bone_id * 0x8));
        return get_transform_position(m_TransformA);
    }
    
    Vector3f get_transform_position(uint64_t transform) {
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

            for (int i = 0; transformIndex >= 0 && i < 200; i++)
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
        //return D3DXVECTOR3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
    }
};


