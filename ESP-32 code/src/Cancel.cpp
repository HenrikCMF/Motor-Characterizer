#include "Cancel.hpp"
Cancel::Cancel()
{
    //initializes a que to be used for cancelling a started measurement
    CancelQue = xQueueCreate(MSG_LENGTH,		
                        	MSG_SIZE);
}

void Cancel::CallToStop()
{
    //Send the signal to stop
    int stop=1;
    xQueueSendToBack(CancelQue, &stop, portMAX_DELAY);
}

bool Cancel::CheckIfShouldStop()
{   
    //Checks if something has been sent to the que, if there is anyting, return true
    bool result=false;
    int received;
    if ( xQueueReceive( CancelQue, &received, 10) !=pdPASS)
    {
    }
    else
    {
        result=true;
    }
    return result;
}