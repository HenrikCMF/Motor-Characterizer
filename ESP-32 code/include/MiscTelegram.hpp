#ifndef MISCTELEGRAM_H
#define MISCTELEGRAM_H
#include "telegram.hpp"
typedef struct //float*100000
{
  DWORD Resistance; //ohm
  DWORD Inductance; //Henry
  DWORD Kt; //I dont know
  DWORD TRipple;
} misctelegramstruct;

class MiscTelegram : public Telegram
{
    private:

    public:
    MiscTelegram(misctelegramstruct &data);
   // ~GraphTelegram
};
#endif