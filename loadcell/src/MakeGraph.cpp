#include "MakeGraph.hpp"
#define SAMPLE_AMOUNT 85
#define QUEUE_LENGTH 4
#define QUEUE_ITEM_SIZE sizeof(void*)
xQueueHandle SampleQue;
TaskHandle_t ADC_Handle=NULL, Torque_Handle=NULL, Sampler_Handle=NULL;
MakeGraph::MakeGraph()
{
  MakeQue();
}

void MakeGraph::MakeQue()
{
  SampleQue = xQueueCreate(QUEUE_LENGTH,		
                        	QUEUE_ITEM_SIZE);
}
void MakeGraph::Start_Graph(void)
{
  //create a thread which runs the measuring algorithm
  xTaskCreate(sampler, NULL, 4096, this, 3, &Sampler_Handle);
}

void MakeGraph::sampler(void* obj)
{
  //initialize all necessary objects
  MakeGraph* newobj=(MakeGraph*)obj;
  RPMcalc speed;
  //create threads for voltage and current
  xTaskCreate(VoltageAndCurrentThread, NULL, 4096, 0, 5, &ADC_Handle);
  xTaskCreate(TorqueThread, NULL, 16384, 0, 3, &Torque_Handle);
  //call the measuring algorithm
  newobj->GetGraphData(speed, newobj);
  //kill all threads involved
  newobj->Stop(speed, newobj);
}
void MakeGraph::VoltageAndCurrentThread(void*)
{
  ADC power{};
  //should never get to this point
  vTaskDelete(NULL);
}
void MakeGraph::TorqueThread(void*)
{
  Torque moment{};
  //should never get to this point
  vTaskDelete(NULL);
}
void MakeGraph::GetGraphData(RPMcalc &speed, MakeGraph* obj)
{
  //function for running the measuring algorithm
  data* received;
  graphtelegramstruct testdata;
  bool runagain=false;
  int i=0, ADCrun=128, Torquerun=10; //iteration
  bool breakout=false;
  int DACVAL=0, prevdac;
  dacWrite(DAC1,DACVAL);
  //ramp up motor
  for(int ii=0; ii<255; ii++)
  {
    dacWrite(DAC2,ii);
    vTaskDelay(30/portTICK_PERIOD_MS);
    if(CheckIfShouldStop())
    {
      Stop(speed, obj);
    }
  }
  //take measurements to prevent early one offs
  for(int ii=0; ii<5; ii++)
  {
    speed.start();
    xQueueSendToBack(ADC::get_que(), &ADCrun, portMAX_DELAY);
    xQueueSendToBack(Torque::get_que(), &Torquerun, portMAX_DELAY);
    for(int j=0; j<4; j++)
    {
      xQueueReceive( MakeGraph::get_que(), &received, portMAX_DELAY);
      delete received;
    }
  }
  delay(0);
  //Start all 4 measurements
  xQueueSendToBack(ADC::get_que(), &ADCrun, portMAX_DELAY);
  xQueueSendToBack(Torque::get_que(), &Torquerun, portMAX_DELAY);
  speed.start();
  while(breakout==false)
  {
    if(CheckIfShouldStop())
    {
      Stop(speed, obj);
    }
    //wait for measurements
    if ( xQueueReceive( MakeGraph::get_que(), &received, portMAX_DELAY) !=pdPASS)
    {
    }
    else
    {
      //store received meaurements
      runagain=storesample((data*)received,testdata,i, speed);
    }
    if(runagain==true)//received everything)
    {
      i++;
      prevdac=DACVAL;
      //increase load
      DACVAL+=get_new_load(i);
      dacWrite(DAC1,DACVAL);
      if(i<5)sleep(1);
      //if 85 measurements have been taken
      if(i==SAMPLE_AMOUNT)
      {
        //transmit data
        GraphTelegram senddata(testdata);
        if(!Serial.availableForWrite())
        {
          Serial.end(false);
          Serial.begin(BAUD_RATE);
        }
        Serial.write(senddata.get_data(), senddata.get_numbytes());
        //end loop
        breakout=true;
      }
      else
      {
        //wait for motor to charge up
        usleep((DACVAL-prevdac)*(DACVAL-prevdac)*10000);
        //call for new measurements
        speed.start();
        xQueueSendToBack(ADC::get_que(), &ADCrun, portMAX_DELAY);
        xQueueSendToBack(Torque::get_que(), &Torquerun, portMAX_DELAY);
        runagain=false;
      }
      
    }
  }
}

bool MakeGraph::storesample(data* measurement, graphtelegramstruct &graph,int i, RPMcalc speed)
{
  static bool S=false, T=false, C=false, V=false;
  const uint multfactor=1000000;
  //store the received measurements in a struct and free their space in memory
  switch(measurement->ID)
  {

    case 0:
      graph.data[i].Speed=(DWORD)round((((speeddata*)measurement)->get_speed())*multfactor);
      delete measurement;
      speed.stop();
      S=true;
      break;
    case 1:
      graph.data[i].Current=(DWORD)round((((currentdata*)measurement)->get_current())*multfactor);
      delete measurement;
      C=true;
      break;
    case 2:
      graph.data[i].Voltage=(DWORD)round((((voltagedata*)measurement)->get_voltage())*multfactor);
      delete measurement;
      V=true;
      break;
    case 3:
      graph.data[i].Torque=(DWORD)round((((torquedata*)measurement)->get_torque())*multfactor);
      delete measurement;
      T=true;
      break;
    default : 
      
      break;
  }
  if(S&T&C&V)
  {
    S=false;
    C=false;
    V=false;
    T=false;
    return true;
  }
  else return false;
}

int MakeGraph::get_new_load(int i)
{
  //increment the load logistically
  if(i==1)
  {
    return 15;
  }
  else if (i<5)
  {
    return 10;
  }
  else if(i>4 && i<40)
  {
    return 3;
  }
  else return 2;

}

void MakeGraph::Stop(RPMcalc& speed, MakeGraph* obj)
{
  //turn off motor and kill all threads involved
  for(int ii=255; ii>0; ii--)
  {
    dacWrite(DAC2,ii);
    vTaskDelay(80/portTICK_PERIOD_MS);
    dacWrite(DAC1,ii);
  }
  ADC::ShutDown();
  vTaskDelete(ADC_Handle);
  vTaskDelete(Torque_Handle);
  speed.end();
  obj->MarkDone();
  vTaskDelay(200);
  vTaskDelete(NULL);
  //should never reach this point
  while(1) vTaskDelay(1/portTICK_PERIOD_MS);
}

QueueHandle_t MakeGraph::get_que(){return SampleQue;}