#ifndef SERIAL_H
#define SERIAL_H
#include "driver/pcnt.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/semphr.h"
#include <HardwareSerial.h>
#define GRAPH_MEASUREMENT 1
#define MISC_MEASUREMENT 2
#define START_GRAPH_MEASUREMENT 3
#define START_CONSTANTS_MEASUREMENT 4
#define LOCK_SHAFT 5
#define USER_INTERACTION_DONE 6
#define STOPEVERYTHING 7
#define MEASURING_KT 8
#define MEASURING_RIPPLE 9
class SerialCom
{
private:
    const int MSG_LENGTH=4;
    const int MSG_SIZE=sizeof(void*);
    xQueueHandle SerialQue;
public:
    SerialCom();
    //send an int to the SerialQue
    void SendToQue(int&);
    //receive an int from the que
    bool ReceiveFromQue(int&);
    //transmit an int over serial
    void SendOnly(int);
    //transmit an int over serial 
    //and await a response
    int SendAndReceive(int);
    //check if there is any incoming serial communication
    bool CheckIfReceived(int&);
};
#endif
