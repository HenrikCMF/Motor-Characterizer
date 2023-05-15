using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace MotorCharactarizer
{
    
    public class Telegram
    {

        public enum telegram_type : byte
        {
            graphdata = 1, //MC sends samples to plot
            miscdata = 2, //MC sends misc data
            starttest = 3, //PC starts graph
            findconstants=4, //PC starts misc data
            lock_shaft=5, //MC sends for user to block rotation
            user_interaction_done=6,//PC tells MC interaction is done
            stopeverything=7, //PC tells MC to stop
            measuring_KT=8,
            measuring_ripple=9
        };
        telegram_type m_ID;
        byte[] m_buffer; 
        int m_size;
        public Telegram()
        {
        }
        public Telegram(telegram_type type, int size)
        {
            //constructs a telegram based on ID and size
            m_buffer = new byte[size];
            m_ID = type;
            m_size = size;
        }
        public byte[] GetData()
        {
            return m_buffer;
        }
        public int GetSize()
        { return m_size; }

        public int Read(SerialPort sp)
        {
            //will read the serial port and save it until there are no more messages
            int size = GetSize();
            int numread = 0;
            while(numread<size)
            {
                numread+= sp.Read(GetData(), numread, GetSize()-numread);
            }
            return numread;
        }
        public void Write(SerialPort sp)
        {
            //will write a telegram to the serial port
            byte[] telegram = new byte[m_size+1];
            telegram[0] = (byte)m_ID;
            if(GetSize()!=0)
                GetData().CopyTo(telegram,1);
            sp.Write(telegram, 0, GetSize()+1);
        }
        public telegram_type GetTelegram_Type()
        { return m_ID; }
    }
}
