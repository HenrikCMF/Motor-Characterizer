#ifndef MUTEX_H
#define MUTEX_H
#include "driver/pcnt.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/semphr.h"
class MutexClass
{
private:
    SemaphoreHandle_t mutex;
    void MutexGive(void);
    SemaphoreHandle_t GetMutex(void);
public:
    bool doneflag;
    MutexClass();
    //used for a thred to mark that it has done everything
    //and it can be deleted
    void MarkDone(void);
    //check if a thread has marked done
    bool CheckIfDone(void);
};
#endif
