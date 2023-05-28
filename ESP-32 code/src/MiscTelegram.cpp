#include "MiscTelegram.hpp"
MiscTelegram::MiscTelegram(misctelegramstruct &data):Telegram(MISC_TELEGRAM, sizeof(misctelegramstruct))
{
    //save measurements as bytes
    append_DWORD(data.Resistance);
    append_DWORD(data.Inductance);
    append_DWORD(data.Kt);
    append_DWORD(data.TRipple);

}