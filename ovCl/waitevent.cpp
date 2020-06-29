#include "waitevent.h"
#include "debug.h"
#include "IVSHMEM.h"

waitEvent::waitEvent(uint16_t vector, HANDLE  event)
{
    this->vector = vector;
    this->event = event;
    start();
}

void waitEvent::run()
{
    while(1)
    {
        if (WaitForSingleObject(event, INFINITE) != WAIT_OBJECT_0)
        {
            DEBUG_ERROR("WaitForSingleObject failed!");
            CloseHandle(event);
            break;
        }
        CloseHandle(event);
        Event(vector, event);
        return;
    }
}
