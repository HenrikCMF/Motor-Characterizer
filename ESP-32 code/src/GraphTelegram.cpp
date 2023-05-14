#include "GraphTelegram.hpp"
#define SAMPLE_AMOUNT 85
GraphTelegram::GraphTelegram(graphtelegramstruct &data):Telegram(GRAPH_TELEGRAM, sizeof(graphtelegramstruct))
{
  //save all data in byte form
    for(int i=0; i<SAMPLE_AMOUNT; i++)
    {
      append_DWORD(data.data[i].Speed);
      append_DWORD(data.data[i].Torque);
      append_DWORD(data.data[i].Current);
      append_DWORD(data.data[i].Voltage);
    }
}
