#include "GetConstants.hpp"

#define QUEUE_LENGTH 4
#define QUEUE_ITEM_SIZE sizeof(void*)
#define LOCK_SHAFT 5
#define USER_DONE 6
#define STOP_READING 7
#define MEASURING_KT 8
#define MEASURING_Ripple 9
extern TaskHandle_t ADC_Handle, Torque_Handle, Sampler_Handle;
using namespace std;
GetConstants::GetConstants(): resistance(0), inductance(0), kt(0), RippleTorque(0)
{
    //Initializes object and creates a thread to handle all the measurements
    xTaskCreate(ThreadFunction, NULL, 4096, this, 3, &Sampler_Handle);
}
void GetConstants::ThreadFunction(void* obj)
{
    int recbuffer=0, to_send=LOCK_SHAFT;
    GetConstants* newobj;
    newobj=(GetConstants*)obj;
    //Create 2 threads, one of voltage and current, another for torque.

    xTaskCreate(VoltageAndCurrentThread, NULL, 4096, 0, 5, &ADC_Handle);
    xTaskCreate(TorqueThread, NULL, 4096, 0, 5, &Torque_Handle);

    //wait for threads to be ready before continueing
    sleep(1);
    int ADCrun=10;
    data* received;
    //take a few ADC readings as the very first measurement is often wrong.
    for(int ii=0; ii<3; ii++)
    {
        xQueueSendToBack(ADC::get_que(), &ADCrun, portMAX_DELAY);
 
        for(int j=0; j<2; j++)
        {
            xQueueReceive( MakeGraph::get_que(), &received, portMAX_DELAY);
 
            delete received;
        }
    }
    //Call function to read resistance and inductance
    newobj->GetRandL(newobj);

    //Send a message to the PC to tell the user to remove the rotor blocker.
    to_send=LOCK_SHAFT;
    newobj->SendToQue(to_send);
    vTaskDelay(200);
    //wait for user to answer
    while(!(newobj->ReceiveFromQue(recbuffer)));
    //if user accepted continue, else stop here.
    if(recbuffer==USER_DONE)
    {
        to_send=MEASURING_KT;
        newobj->SendToQue(to_send);
        newobj->GetKT(newobj);
        to_send=MEASURING_Ripple;
        newobj->SendToQue(to_send);
        newobj->GetT_Ripple(newobj);
    }
    newobj->TransmitData();
    newobj->stop(newobj);
}
void GetConstants::GetRandL(GetConstants* obj)
{
    struct timeval time1, time2, measuredtime;
    double indvoltage=-1, indcurrent, Rvoltage, Rcurrent;
    int voltagetuner=1, samples=120;
    dacWrite(DAC1,0);
    //find a voltage between 1.5 and 2.5V, to avoid short circuits.
    dacWrite(DAC2,255);
    while(!(indvoltage>1.5 && indvoltage<2.5))
    {
        dacWrite(DAC2,voltagetuner);
        delay(400);
        GetVandI(indvoltage, indcurrent, samples);
        voltagetuner+=1;
        if(indvoltage>4) voltagetuner=1;
    }
    samples=1;
    float div=0;
    double tempinductance, suminductance=0;
    //Create 160 individual pulses that go into the motor, each pulse lasts 800us
    //each pulse has a half second delay in between to allow for complete discharge of the motor
    for(int j=0; j<160; j++)
    {
        if(CheckIfShouldStop())
        {
            obj->stop(obj);
        }
        //turn off pulse
        dacWrite(DAC2,0);
        delay(500);
        gettimeofday(&time1,NULL);
        //start a pulse;
        dacWrite(DAC2,voltagetuner);
        usleep(1);
        //measure voltage and current 8 times
        for(int i=0; i<8; i++)
        {
            GetVandI(indvoltage, indcurrent, samples);
            gettimeofday(&time2,NULL);
            //calculate how much time passed between pulse started and measurement finished.
            measuredtime.tv_sec=time2.tv_sec-time1.tv_sec;
            measuredtime.tv_usec=time2.tv_usec-time1.tv_usec;
            tempinductance=1000.0*(indvoltage*((double)measuredtime.tv_sec+((double)measuredtime.tv_usec/1000000.0)))/indcurrent; //very rough estimate converted to mH
            if((tempinductance>5000) || (tempinductance<0))
            {
                continue;
            }
            else
            {
                suminductance+=tempinductance;
                div++;
            }
        }
        
    }
    if(div>0) suminductance/=div;
    inductance=abs(suminductance);
    //let the motor become steady state by waiting
    sleep(10);
    samples=1000;
    GetVandI(Rvoltage, Rcurrent, samples);
    //calculate resistance and inductance based on measured values.
    resistance=Rvoltage/Rcurrent;

   
    dacWrite(DAC2,0);
    dacWrite(DAC1,0);
}

void GetConstants::GetKT(GetConstants* obj)
{
    dacWrite(DAC1,0);
    //ramp up motor to full speed
    for(int ii=0; ii<255; ii++)
    {
        if(CheckIfShouldStop())
        {
            obj->stop(obj);
        }
        dacWrite(DAC2,ii);
        vTaskDelay(30/portTICK_PERIOD_MS);
    }
    //wait for steady state
    sleep(2);
    double speed, voltage, current;
    //measure speed and calculate rads/s
    GetSpeed(speed);
    speed=(speed * 2.0 * M_PI) /  60.0;
    //measure voltage and current
    GetVandI(voltage, current, 120);
    //calculate kt
    kt=(voltage-resistance*current)/speed;
    //stop motor
    for(int ii=255; ii>0; ii--)
    {
        if(CheckIfShouldStop())
        {
            obj->stop(obj);
        }
        dacWrite(DAC2,ii);
        vTaskDelay(30/portTICK_PERIOD_MS);
    }
}
void get_torque(float& array) //used for finding torque ripple
{
    static int count=0;
    int average=1;
    data* received;
    //call for a single torque measurement
    xQueueSendToBack(Torque::get_que(), &average, portMAX_DELAY);
    delay(1); //feed watchdog
    //receive torque measurement 
    xQueueReceive( MakeGraph::get_que(), &received, portMAX_DELAY);
    //save the measured torque
    array=((torquedata*)received)->get_torque();
    delete received;
}

void GetConstants::GetT_Ripple(GetConstants* obj)
{
    
    int num=1000, averaging=30;
    double indvoltage=-1, indcurrent;
    int TorqueH[averaging], TorqueL[averaging];
    //fill arrays with not possible values
    for(int i=0; i<averaging; i++)
    {
        TorqueH[i]=-1;
        TorqueL[i]=-1;
    }
    //full load
    dacWrite(DAC1,255);
    
    int voltagetuner=0;
    //find a low voltage over the test motor
    while(!((indvoltage>1.8) && (indvoltage<2.3)))
    {
        dacWrite(DAC2,voltagetuner);
        delay(500);
        GetVandI(indvoltage, indcurrent, 120);
        voltagetuner++;
        if(indvoltage>4) voltagetuner=1;
    }
    double average=0;
    float *samples=new float[num];
    sleep(1);
    //measure torque 1000 times and save measurements
    for_each(samples,samples+num, get_torque);
    //add all torque measurements together to create an average
    for_each(samples,samples+num, [&average](float sample){average+=sample;});
    average/=num;
    float* i1;
    //find the 30 largest peaks and valleys and save them
    for(int i=0; i<averaging;i++)
    {
        i1=max_element(samples,samples+num);
        TorqueH[i]=*i1;
        *i1=average;
        i1=min_element(samples,samples+num);
        TorqueL[i]=*i1;
        *i1=average;
    }
    delete samples;
    float trueL=0, trueH=0;
    //calculate averages for largest peaks and largest valleys separately
    for_each(TorqueH,TorqueH+averaging, [&trueH](float sample){trueH+=sample;});
    for_each(TorqueL,TorqueL+averaging, [&trueL](float sample){trueL+=sample;});
    trueL/=averaging;
    trueH/=averaging;
    //calculate ripple torque and turn off motor
    RippleTorque=(trueH-trueL);
    for(int i=voltagetuner; i>-1; i--)
    {
        dacWrite(DAC2, i);
    }
    dacWrite(DAC1, 0);
}

void GetConstants::GetVandI(double& V, double& C, int samples)
{
    //measure voltage and current with an average of amount given in samples
    data* received;
    int ADCrun=samples;
    bool cc=false, vv=false;
    //ask for measurements
    xQueueSendToBack(ADC::get_que(), &ADCrun, portMAX_DELAY);
    while(!(cc&&vv))
    {
        //wait to receive measurements
        if ( xQueueReceive( MakeGraph::get_que(), &received, portMAX_DELAY) !=pdPASS)
        {
        }
        else
        {
            //when received, save it and clear space in memory
            switch(received->ID)
            {
                case 1:
                    C=((currentdata*)received)->get_current();
                    delete received;
                    cc=true;
                    break;
                case 2:
                    V=((voltagedata*)received)->get_voltage();
                    delete received;
                    vv=true;
                    break;
                default : 
                break;
            }
        }
    }
    
}

void GetConstants::GetSpeed(double& S)
{
    //measure speed
    data* received;
    int ADCrun=64;
    RPMcalc findspeed;
    //start speed measurement
    findspeed.start();
    xQueueReceive( MakeGraph::get_que(), &received, portMAX_DELAY);
    findspeed.start();
    //wait for measurement to arrive
    if ( xQueueReceive( MakeGraph::get_que(), &received, portMAX_DELAY) !=pdPASS)
    {
    }
    else
    {
        //save speed measurement
        S=((speeddata*)received)->get_speed();
        delete received;
        findspeed.end();
    }
    
}

void GetConstants::TransmitData(void)
{
    //save the measured values in a misctelegram struct
    misctelegramstruct data_to_be_transmitted;
    data_to_be_transmitted.Inductance=(DWORD)(inductance*100000);
    data_to_be_transmitted.Resistance=(DWORD)(resistance*100000);
    data_to_be_transmitted.Kt=(DWORD)(kt*100000);
    data_to_be_transmitted.TRipple=(DWORD)(RippleTorque*100000);
    //use misctelegram class to save the data in bytes
    MiscTelegram sendstuff(data_to_be_transmitted);
    //check if there is still a connection
    if(!Serial.availableForWrite())
    {
        Serial.end(false);
        Serial.begin(BAUD_RATE);
    }
    //send measurements
    Serial.write(sendstuff.get_data(), sendstuff.get_numbytes());
}

void GetConstants::stop(GetConstants* obj)
{
    //ends the algorithm and kills all threads involved
    dacWrite(DAC1, 0);
    for(int ii=128; ii>0; ii--)
    {
        dacWrite(DAC2,ii);
        vTaskDelay(80/portTICK_PERIOD_MS);
    }
    ADC::ShutDown();
    vTaskDelete(ADC_Handle);
    vTaskDelete(Torque_Handle);
    obj->MarkDone();
    vTaskDelete(Sampler_Handle);
    vTaskDelete(NULL);
}