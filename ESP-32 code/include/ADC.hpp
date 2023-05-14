#ifndef ADC_H
#define ADC_H
#include <Arduino.h>
#include <driver/adc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "data.hpp"
#include "esp_adc_cal.h"
using namespace std;
#define ADC_CHANNEL_VOLTAGE_PLUS ADC1_CHANNEL_6
#define ADC_CHANNEL_CURRENT ADC1_CHANNEL_3
#define ADC_CHANNEL_VOLTAGE_MINUS ADC2_CHANNEL_7    /*!< ADC2 channel 7 is GPIO27 (ESP32), GPIO18 (ESP32-S2) */
#define DATAID_CURRENTDATA 1
#define DATAID_VOLTAGEDATA 2
#define ADCQUEUE_LENGTH 1
#define ADCQUEUE_ITEM_SIZE sizeof(int)
//static xQueueHandle ADCQue = NULL;

class ADC
{
    private:
        float average;
        float vcc;
        float currentcalibration;
        //read both voltages and returns the difference
        float readVoltage(void);
        //returns current
        float readCurrent(void);
        //thread function which handles requests via ques
        void powerMeas(void);
        //measure current and voltage concurrently (readvoltage must have been called before this)
        void ReadSlope(void);
        //thread function which handles ADC2 requests
        static void ADC2Thread(void* obj);
        
    public:
        
        ADC();
        ~ADC();
        //will kill the ADC2 thread
        static void ShutDown(void);
        static QueueHandle_t get_que();
};

class voltagedata : data
{
    private:
    float sample;
    public:
    
    inline voltagedata(float voltage) : data(DATAID_VOLTAGEDATA)
    {
        sample=voltage;
    };
    inline voltagedata(void): data()
    {

    }
    inline float get_voltage()
    {
        return sample;
    }
};

class currentdata : data
{
    private:
    float sample;
    public:
    
    inline currentdata(float current) : data(DATAID_CURRENTDATA)
    {
        sample=current;
    };
    inline currentdata(void): data()
    {

    }
    inline float get_current()
    {
        return sample;
    }
};
#endif