//
// Created by root on 7/8/20.
//
#include "common/ivshmem_com.h"


bool initArray(ESPObjectArray *a, int initialSize)
{
    a->array = (ESPObject *)malloc(initialSize * sizeof(ESPObject));
    if (!a->array)
        return false;

    memset(a->array, 0, initialSize * sizeof(ESPObject));

    a->used = 0;
    a->size = initialSize;
    return true;
}

void insertArray(ESPObjectArray *a, ESPObject element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (ESPObject *)realloc(a->array, a->size * sizeof(ESPObject));
    }
    a->array[a->used++] = element;
}

void clearArray(ESPObjectArray *a)
{
    memset(a->array, 0, a->size * sizeof(ESPObject));
    a->used = 0;
}

void freeArray(ESPObjectArray *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void send_data(IVSHMEM* shm, ESPObjectArray* data)
{
    void* memory = shm->mem;
    size_t size = data->size;
    ESPObject * array = data->array;
    for (size_t t = 0; t < size; ++t) {
        memcpy((void*)((uintptr_t)memory + t * sizeof(ESPObject)),
                array + t,
                sizeof(ESPObject));
    }
}