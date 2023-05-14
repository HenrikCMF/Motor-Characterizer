/*
COPYRIGHT HENRIK FREDERIKSEN 

PLEASE DONT STEAL

IF YOU DO; I WILL SUE
*/
//#include "MakeGraph.hpp"
#include "GetConstants.hpp"
#include "SerialCom.hpp"
#include <Arduino.h>
#include "driver/uart.h"
using namespace std;

#define START_GRAPH_MEASUREMENT 3
#define START_CONSTANTS_MEASUREMENT 4
#define UART_RX_PIN 16 // GPIO2
#define UART_TX_PIN 17 // GPIO4
void setup() 
{
  Serial.begin(BAUD_RATE);
  dacWrite(DAC2,0);
  dacWrite(DAC1,0);
}


void loop() 
{
  int recbuffer=0;
  int temp=0;
  MakeGraph* Generate=NULL;
  GetConstants* Test=NULL;
  SerialCom messenger;
  while(1)
  {
    //check if there is any serial communication
    if(messenger.CheckIfReceived(recbuffer))
    {
      switch(recbuffer)
      {
        case START_GRAPH_MEASUREMENT:
          //initialize a MakeGraph object and start measurements
          if(Generate==NULL)
          {
            Generate=new MakeGraph;
            Generate->Start_Graph();
          }
          else //if it has already started, call to stop
          {
            Generate->CallToStop();
          }
          break;
        case START_CONSTANTS_MEASUREMENT:
          if(Test==NULL)
          {
            Test=new GetConstants;
          }
          else
          {
            Test->CallToStop();
          }
          break;
      }
      recbuffer=0;
    }
    if(Generate) // a MakeGraph object exists, check if it has marked itself done
    { 
      if(Generate->CheckIfDone())
      {
        Generate->doneflag=false;
        delete Generate;
        Generate=NULL;
      }
    }
    delay(0);
    if(Test) // if a getconstants object exists
    { 
      int tosend;
      if(Test->ReceiveFromQue(tosend)) //check if it wants to communicate with the PC
      {
        if(tosend==LOCK_SHAFT) 
        {
          recbuffer=messenger.SendAndReceive(tosend);
          Test->SendToQue(recbuffer);
        }
        else messenger.SendOnly(tosend);
        recbuffer=0;
      }
      if(Test->CheckIfDone()==true) //check if it has marked itself done
      {
        Test->doneflag=false;
        delete Test;
        Test=NULL;
      }
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }

}

