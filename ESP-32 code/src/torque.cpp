#include "torque.h"
#include "MakeGraph.hpp"
#define GRAVITATIONAL_ACCELERATION 9.82
#define LOAD_ARM_LENGTH 10.5
xQueueHandle TorqueQue;
Torque::Torque()
{
    TorqueQue = xQueueCreate(ADCQUEUE_LENGTH,		/* The number of items the queue can hold. */
                        	ADCQUEUE_ITEM_SIZE);	/* The size of each item the queue holds. */
    HX711 scale;
    scale.begin(35, 32, 128); //pin GPIO35 and GPIO32
    while (scale.is_ready()==false);
    scale.tare(20);
    int average;
    while(1)
    {
        if ( xQueueReceive( Torque::get_que(), &average, portMAX_DELAY) !=pdPASS)
        {
            delay(0);
        }
        else
        {
            torquedata *moment=NULL;
            
            float reading = scale.get_units(average)/-287.24; //get in grams
            delay(0);
            //Serial.println(reading);
            //weight in grams multiplied by gravitational acceleration in m/s^2
            //multiplied with the length of the arm in cm
            //divided by 1000 so the unit ends up being N*cm
            reading=(reading*GRAVITATIONAL_ACCELERATION*LOAD_ARM_LENGTH)/1000.0;
            moment=new torquedata(reading);
            xQueueSend(MakeGraph::get_que(),&moment,portMAX_DELAY);
        }
    }
}
QueueHandle_t Torque::get_que(){return TorqueQue;}