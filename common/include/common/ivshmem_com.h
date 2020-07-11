#pragma once
#include <stdbool.h>
#include <vmath.h>

extern "C" {
#include "common/ivshmem.h"
#include "common/debug.h"
}


typedef struct
{
    char pName[100];
    float x, y;
    float xHead, yHead;
    float distance;
    float inGameDistance;
    float r, g, b;
    bool drawBones;
    std::pair<Vector2f, Vector2f> bones[10];
}ESPObject;

typedef struct
{
    ESPObject *array;
    int used;
    int size;
} ESPObjectArray;

bool initArray(ESPObjectArray *a, int initialSize);
void insertArray(ESPObjectArray *a, ESPObject element);
void clearArray(ESPObjectArray *a);
void freeArray(ESPObjectArray *a);

void send_data(IVSHMEM* shm, ESPObjectArray* data);