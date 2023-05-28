#ifndef SPEED_H
#define SPEED_H
#include "speeddata.hpp"
#include <Arduino.h>
#define count_pin 36
typedef uint8_t BYTE;
/* The queue used by the queue send and queue receive tasks. */

//extern xQueueHandle SampleQue;
/* The semaphore (in this case binary) that is used by the FreeRTOS tick hook
 * function and the event semaphore task.
 */
class RPMcalc
{
    private:
    public:
        volatile int newmeas;
        RPMcalc();
        //start a new measurement
        void start(void);
        //stop measuring
        void stop(void);
        //detach interrupt
        void end(void);
};
#endif