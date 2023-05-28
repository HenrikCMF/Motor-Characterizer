/*
Measures and stores torque samples using the HX711 library
Written by Henrik Frederiksen
*/
#ifndef TORQUE_H
#define TORQUE_H

#include <Arduino.h>
#include "HX711.hpp"
#include "data.hpp"
#define DATAID_TORQUEDATA 3
#define ADCQUEUE_LENGTH 1
#define ADCQUEUE_ITEM_SIZE sizeof(int)
class Torque
{
    private:
    public:
        //when constructor is called, it will stay in it until the thread is deleted
        Torque();
        //returns the torque que
        static QueueHandle_t get_que();
};
class torquedata : data
{
    private:
    float sample;
    public:
    
    inline torquedata(float received) : data(DATAID_TORQUEDATA)
    {
        sample=received;
    };
    inline torquedata(void): data()
    {

    }
    inline float get_torque()
    {
        return sample;
    }
};

#endif