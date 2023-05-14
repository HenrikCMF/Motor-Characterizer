#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "driver/pcnt.h"
#include <stdio.h>
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
#include "MakeGraph.hpp"
#include "MiscTelegram.hpp"
#include "SerialCom.hpp"
#include <algorithm>

class GetConstants: public MakeGraph, public SerialCom
{
    private:
        float resistance;
        double inductance;
        double kt;
        double RippleTorque;
        //Run an algorithm to find resistance and inductance
        void GetRandL(GetConstants* obj);
        //measure voltage and current
        void GetVandI(double&, double&, int samples);
        //Measure the torque constant KT
        void GetKT(GetConstants*);
        //measure speed
        void GetSpeed(double&);
        //run an algorithm to find the amount of torque ripple
        void GetT_Ripple(GetConstants*);
        //transmit all the found constants
        void TransmitData(void);
        //function which will use all the other functions to find all the data
        static void ThreadFunction(void*);
        //end all threads in the object
        void stop(GetConstants*);

    public:

        GetConstants();
        
};
#endif