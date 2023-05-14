#include "SerialCom.hpp"

SerialCom::SerialCom()
{
    SerialQue = xQueueCreate(MSG_LENGTH,		
                        	MSG_SIZE);
}

bool SerialCom::ReceiveFromQue(int& output)
{
    bool result=false;
    if ( xQueueReceive( SerialQue, &output, 10) !=pdPASS)
    {
    }
    else
    {
        result=true;
    }
    return result;
}

void SerialCom::SendToQue(int& input)
{
    xQueueSendToBack(SerialQue, &input, portMAX_DELAY);
}

 void SerialCom::SendOnly(int message)
 {
    Serial.write(message);
 }
int SerialCom::SendAndReceive(int input)
{
    Serial.write(input);
    int recbuffer;
    while(!(Serial.available()));
    recbuffer=Serial.read();
    return recbuffer;
}

bool SerialCom::CheckIfReceived(int& message)
{
    int received;
    bool result=false;
    if(Serial.available())
    {
        message=Serial.read();
        result=true;
    }
    return result;
}
