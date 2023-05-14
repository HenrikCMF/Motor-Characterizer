#ifndef CANCEL_H
#define CANCEL_H
#include "driver/pcnt.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/semphr.h"

class Cancel
{
private:
    const int MSG_LENGTH=4;
    const int MSG_SIZE=sizeof(void*);
    xQueueHandle CancelQue;
public:
    Cancel();
    //use if a given object should end itself
    void CallToStop(void);
    //an object can check for itself if it has been called to end
    bool CheckIfShouldStop(void);
};
#endif
