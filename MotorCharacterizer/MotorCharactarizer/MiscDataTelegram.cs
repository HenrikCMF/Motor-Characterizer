using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
namespace MotorCharactarizer
{
    public struct MiscDatastruct //float*100000
    {
        public UInt32 Resistance; //RPM
        public UInt32 Inductance; //Nm
        public UInt32 Kt; //A
        public UInt32 TRipple; //V
    }
    internal class MiscDataTelegram:Telegram 
    {
        public MiscDataTelegram(): base(telegram_type.miscdata, Marshal.SizeOf(typeof(MiscDatastruct)))
        { 

        }

        public Boolean get_data( ref MiscDatastruct data) //will save all constants in the given struct
        {
            Boolean result = false;
            int index = 0;
            if (Marshal.SizeOf(typeof(MiscDatastruct)) <= base.GetSize())
            {
                data.Resistance = BitConverter.ToUInt32(base.GetData(), index);
                index += sizeof(UInt32);
                data.Inductance = BitConverter.ToUInt32(base.GetData(), index);
                index += sizeof(UInt32);
                data.Kt = BitConverter.ToUInt32(base.GetData(), index);
                index += sizeof(UInt32);
                data.TRipple = BitConverter.ToUInt32(base.GetData(), index);
                result = true;
            }
            return result;
        }
    }
}
