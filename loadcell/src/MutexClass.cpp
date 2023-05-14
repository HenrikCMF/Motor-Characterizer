#include "MutexClass.hpp"
#include "Arduino.h"
MutexClass::MutexClass()
{
    mutex=xSemaphoreCreateBinary();
    MutexGive();
    doneflag=false;
}
void MutexClass::MutexGive(void)
{
    xSemaphoreGive(mutex);
}

SemaphoreHandle_t MutexClass::GetMutex(void)
{
    return mutex;
}

void MutexClass::MarkDone(void)
{
    //ensures thread safety for setting the doneflag
    xSemaphoreTake( mutex,portMAX_DELAY);
    doneflag=true;
    MutexGive();
}

bool MutexClass::CheckIfDone(void)
{
    bool result=false;
    //ensures thread safety for checking the doneflag
    if( xSemaphoreTake( mutex, ( TickType_t ) 10 ) == pdTRUE )
    {
        
        if(doneflag)
        {
            result=true;
        }
        MutexGive();
    }
    return result;
}