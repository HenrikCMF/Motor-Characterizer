#include "ADC.hpp"
#include "MakeGraph.hpp"
bool run=false;
xQueueHandle ADCQue;
TaskHandle_t ADC2_Handle=NULL;
esp_adc_cal_characteristics_t adc1_chars, adc2_chars;
xQueueHandle ADC2Que_send, ADC2Que_receive;
ADC::ADC() 
{
    //initializes all 3 ADC channels
    average=128.0;
    float ADC_value;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL_CURRENT,ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC_CHANNEL_VOLTAGE_PLUS,ADC_ATTEN_DB_11);
    adc2_config_channel_atten(ADC_CHANNEL_VOLTAGE_MINUS,ADC_ATTEN_DB_11);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, (adc_bits_width_t)ADC_WIDTH_BIT_12, 0, &adc1_chars);
    esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, (adc_bits_width_t)ADC_WIDTH_BIT_12, 0, &adc2_chars);
    //Finds the current value measured by the current sensor, while no current is flowing
    //to be used as calibration for all future current measurements.
    for (int i=0; i<average; i++)
    {
         ADC_value+=adc1_get_raw(ADC_CHANNEL_CURRENT);
    }
    currentcalibration=(float)ADC_value/average;
    currentcalibration=esp_adc_cal_raw_to_voltage(currentcalibration, &adc1_chars)/1000.0;
    currentcalibration=currentcalibration/0.066;
    //Initializes Que for communication with ADC thread
    ADCQue = xQueueCreate(ADCQUEUE_LENGTH,		/* The number of items the queue can hold. */
                        	ADCQUEUE_ITEM_SIZE);	/* The size of each item the queue holds. */
    ADC2Que_receive = xQueueCreate(ADCQUEUE_LENGTH,		
                            ADCQUEUE_ITEM_SIZE);	
    ADC2Que_send=xQueueCreate(ADCQUEUE_LENGTH,		
                        	sizeof(float));
    xTaskCreate(ADC2Thread, NULL, 4096, this, 6, &ADC2_Handle); //create a thread to handle requests for measurements from ADC2
    powerMeas();
};
ADC::~ADC()
{
}
void ADC::ShutDown(void)
{
    vTaskDelete(ADC2_Handle);
}
void ADC::ADC2Thread(void* obj)
{
    int received, ADC2_read;
    double ADC2_value=0;
    float voltage;
    ADC* newobj=(ADC*)obj;
    while(1)
    {
        //wait in que for infinite time until any message is received
        if ( xQueueReceive( ADC2Que_receive, &received, portMAX_DELAY) !=pdPASS)
        {
            delay(0);
        }
        //when message is received, take an amount of measurements equal to the current 
        //value stored in the average variable of the ADC object and take the average
        else
        {
            ADC2_value=0;
            for (int i=0; i<newobj->average; i++)
            {
                adc2_get_raw( ADC_CHANNEL_VOLTAGE_MINUS, ADC_WIDTH_12Bit, &ADC2_read);
                ADC2_value+=ADC2_read;
                
            }
            voltage=ADC2_value/(newobj->average);
            //calibrate measurement and send it back
            voltage= esp_adc_cal_raw_to_voltage(voltage, &adc2_chars)/1000.0;
            xQueueSend(ADC2Que_send, &voltage,portMAX_DELAY);
        }
    }
    
}
float ADC::readVoltage(void)
{

    double ADC_value=0;
    
    float voltageplus=0, voltageminus=0;
    //tell ADC2 to take a measurement
    xQueueSend(ADC2Que_receive, &average,portMAX_DELAY);
    //take measurements with ADC2 and average
    for (int i=0; i<average; i++)
    {
        ADC_value+=adc1_get_raw(ADC_CHANNEL_VOLTAGE_PLUS);
    }
    voltageplus=(ADC_value/average);
    
    //Uses set compensation to get more accurate values:
    voltageplus= esp_adc_cal_raw_to_voltage(voltageplus, &adc1_chars)/1000.0;
    vcc=voltageplus*9.26; //used for concurrect measurements for calculating inductance
    xQueueReceive( ADC2Que_send, &voltageminus, portMAX_DELAY); //receive ADC2 measurement
    //returns the two voltages subtracted, which is the voltage that will be seen across the motor
    return ((voltageplus-voltageminus)*9.26);
}

float ADC::readCurrent(void)
{
    float ADC_value=0;
    float current=0;
    //use ADC1 to read the current
    for (int i=0; i<average; i++)
    {
         ADC_value+=adc1_get_raw(ADC_CHANNEL_CURRENT);
    }
    //take average and calibrate measurements
    current=(float)ADC_value/average;
    current=esp_adc_cal_raw_to_voltage(current, &adc1_chars)/1000.0;
    //return current when it has been converted to amperes
    return (current/0.066)-currentcalibration;
}

void ADC::ReadSlope(void)
{
    //used for reading voltage and current concurrently
    currentdata *current=NULL;
    voltagedata *voltage=NULL;
    float voltageminus=0;
    //measure call for ADC2 measurements
    xQueueSend(ADC2Que_receive, &average,portMAX_DELAY);
    //read current with ADC1
    current=new currentdata(ADC::readCurrent());
    //send current measurement
    xQueueSend(MakeGraph::get_que(),&current,portMAX_DELAY);
    //receive ADC2 measurement
    xQueueReceive( ADC2Que_send, &voltageminus, portMAX_DELAY);
    //send voltage measurement
    voltage=new voltagedata(vcc-(voltageminus*9.26));
    xQueueSend(MakeGraph::get_que(),&voltage,portMAX_DELAY);
}

QueueHandle_t ADC::get_que(){return ADCQue;}
void ADC::powerMeas(void)
{
    //ADC thread waits for inputs in its que, and will then read voltage and current
    //These values will be sent in the que to be received by the sampler thread.
    int received; 
    while(1)
    {
        if ( xQueueReceive( ADC::get_que(), &received, portMAX_DELAY) !=pdPASS)
        {
            delay(0);
        }
        else
        {
            //if no averaging is needed, take measurements concurrently
            average=received;
            if(average==1)
            {
                ReadSlope();
            }
            else
            {
                voltagedata *voltage=NULL;
                currentdata *current=NULL;
                current=new currentdata(ADC::readCurrent());
                xQueueSend(MakeGraph::get_que(),&current,portMAX_DELAY);
                voltage=new voltagedata(ADC::readVoltage());
                xQueueSend(MakeGraph::get_que(),&voltage,portMAX_DELAY);
            }
            
        }
    }
}