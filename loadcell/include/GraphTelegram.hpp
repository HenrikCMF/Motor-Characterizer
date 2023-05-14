#ifndef GRAPHTELEGRAM_H
#define GRAPHTELEGRAM_H


#include "telegram.hpp"

typedef struct //float*100000
{
  DWORD Torque; //Nm
  DWORD Speed; //RPM
  DWORD Current; //A
  DWORD Voltage; //V
} GraphData;

typedef struct
{
  GraphData data[85];
} graphtelegramstruct;

class GraphTelegram : public Telegram
{
    private:

    public:
    GraphTelegram(graphtelegramstruct &data);
   // ~GraphTelegram
};
#endif