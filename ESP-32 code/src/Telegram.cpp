#include "Telegram.hpp"
#include <Arduino.h>
Telegram::~Telegram()
{
    if(array!=NULL)
    {
        delete array;
        array=NULL;
    }
};
void Telegram::DWORD_to_BYTE(DWORD input, uint8_t *array) //saves DWORD in little engine format
{
        array[0]=input&0xFF;
        array[1]=((input>>8)&0xFF);
        array[2]=((input>>16)&0xFF);
        array[3]=((input>>24)&0xFF);;
}
Telegram::Telegram(int id, int numbytes)
{
    ID=id;
    array=new BYTE[(sizeof(BYTE)+numbytes)]; //allocate number of bytes in telegram, in memory
    array[0]=id;
    index=1;
    size=sizeof(BYTE)+numbytes;
    
};
bool Telegram::append_DWORD(DWORD input)
{
    //store a DWORD in memory
    bool result=false;
    if(index+sizeof(DWORD)<=size)
    {
        DWORD_to_BYTE(input,&array[index]);
        index=index+sizeof(DWORD);
        result=true;
    }
    return result;
}