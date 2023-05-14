#include "speed.h"
#include "MakeGraph.hpp"
struct timeval time1, time2, measuredtime, debouncer;
bool running=false;
#define ROTATIONS 15
static double waittime=12000.0;
volatile int newrun=0;
void IRAM_ATTR read() //interrupt routine for the hall effect sensor
{
    if (running==true)
    {
        if(newrun)
        {
            //save a new time for measurements
            gettimeofday(&time1, NULL);
            time2=time1; 
            newrun=0;
        }
        speeddata *RPM=NULL;
        static double RPMtemp=0;
        
        static int count_var=0, calibrate=0;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        gettimeofday(&debouncer, NULL);
        //debounce the signal
        if((double)(debouncer.tv_sec+debouncer.tv_usec/1000000.0)>((double)(time2.tv_sec+(time2.tv_usec+waittime)/1000000.0)))
        {
            count_var++;
            time2=debouncer;
        }
        //when 15 pulses have been counted
        if (count_var==ROTATIONS)
        {
            //Calculate how much time has passed since measurement start
            measuredtime.tv_sec=time2.tv_sec-time1.tv_sec;
            measuredtime.tv_usec=time2.tv_usec-time1.tv_usec;
            //calculate RPM
            RPMtemp=(ROTATIONS*60)/((double)measuredtime.tv_sec+((double)measuredtime.tv_usec/1000000.0));
            RPM=new speeddata(RPMtemp);
            xQueueSendToBackFromISR(MakeGraph::get_que(),&RPM,&xHigherPriorityTaskWoken);
            running=false;
            time1=time2;
            count_var=0;
        }
        
    }
    
}

RPMcalc::RPMcalc(): newmeas(0)
{
    attachInterrupt(count_pin, read, RISING);
}

void RPMcalc::start(void)
{

    running=true;
    newrun=1;

}
void RPMcalc::stop(void)
{
    running=false;
}
void RPMcalc::end(void)
{
    running=false;
    detachInterrupt(count_pin);
}