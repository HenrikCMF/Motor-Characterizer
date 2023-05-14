#ifndef MAKEGRAPH_H
#define MAKEGRAPH_H
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/queue.h"
#include "ADC.hpp"
#include "speeddata.hpp"
#include "GraphTelegram.hpp"
#include "MiscTelegram.hpp"
#include "freertos/task.h"
#include "torque.h"
#include "speed.h"
#include "MutexClass.hpp"
#include "Cancel.hpp"
#define BAUD_RATE 115200
class MakeGraph:public MutexClass,  public Cancel
{
    private:
        //returns a value used for incrementing the DAC for the load
        int get_new_load(int i);
        //ends all threads in the object
        void Stop(RPMcalc&, MakeGraph* obj);
        //function which runs the graph measuring algorithm
        static void sampler(void*);
        //calls the voltage, current and torque threads and implements delays
        void GetGraphData(RPMcalc &speed, MakeGraph* obj);
        //store the measured samples
        bool storesample(data* measurement, graphtelegramstruct &graph,int i, RPMcalc speed);
    public:
        
        //static xQueueHandle SampleQue;
        //MutexClass mutex;
        MakeGraph();
        //initialize the SampleQue
        void MakeQue(void);
        //a function which will forever wait for calls to measure voltage and current
        static void VoltageAndCurrentThread(void*);
        //a function which will forever wait for calls to measure torque
        static void TorqueThread(void*);
        //returns the sample que
        static  QueueHandle_t get_que();
        //starts a new thread which will run the graph measuring algorithm
        void Start_Graph(void);
        
};
#endif