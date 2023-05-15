using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
namespace MotorCharactarizer
{
    public struct GraphDatastruct //float*100000
    {
        public UInt32 Speed; //RPM
        public UInt32 Torque; //Nm
        public UInt32 Current; //A
        public UInt32 Voltage; //V
    }
    
public class GraphDataTelegram: Telegram
    {
        public GraphDataTelegram(): base(telegram_type.graphdata, Marshal.SizeOf(typeof(GraphDatastruct)) * 85)
        {
 
        }
        public Boolean get_data(int i, ref  GraphDatastruct data) //will fetch a set of measurement values based on index given
        {
            Boolean result = false;
            int index = Marshal.SizeOf(typeof(GraphDatastruct)) * i;
            if (index + Marshal.SizeOf(typeof(GraphDatastruct)) <= base.GetSize())
            {
                data.Speed = BitConverter.ToUInt32(base.GetData(), index);
                index += sizeof(UInt32);
                data.Torque = BitConverter.ToUInt32(base.GetData(), index);
                index += sizeof(UInt32);
                data.Current = BitConverter.ToUInt32(base.GetData(), index);
                index += sizeof(UInt32);
                data.Voltage = BitConverter.ToUInt32(base.GetData(), index);
                result = true;
            }
            return result;
        }
    }
}
