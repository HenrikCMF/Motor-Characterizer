using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Configuration;
using System.IO.Ports;
using System.Windows.Forms.DataVisualization.Charting;

namespace MotorCharactarizer
{
    public partial class Form1 : Form
    {
        string m_TS = "Prediction";
        string EFF = "Efficiency";
        string m_C = "Current";
        string m_V = "Voltage";
        string KTc = "KT*Current";
        string m_MW = "Mechanical power";
        string m_EW = "Electrical power";
        public Form1()
        {
            //initializes when the program is started
            InitializeComponent();
            InitPortName();
            string port = ConfigurationManager.AppSettings["portname"];
            comboPortName.SelectedIndex = comboPortName.FindStringExact(port);
            initGraphsTS();
            initGraphsEFF();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //when the measure button is clicked on the graph page
            SetError("");
            OpenSerialPort();
            //when button is pressed and a serial connection is available
            // transmit "3" which means start measurement
            if (serialPort.IsOpen) 
            {
                //send start graph measurement over serial communication
                int MyInt = (int)Telegram.telegram_type.starttest;
                byte[] b = BitConverter.GetBytes(MyInt);
                serialPort.Write(b, 0, 1);
                //code for switching between if the button cancels or starts measurement
                if(Btn_Start.Text=="Measure")
                {
                    Btn_Start.Text = "Cancel Measurement";
                    button1.Enabled = false;
                }
                else
                {
                    Btn_Start.Text = "Measure";
                    button1.Enabled = true;
                }
            }
            
        }
        private void InitPortName()
        {
            //displays serial port names in drop down list
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();
            comboPortName.Items.Clear();
            //comboPortName.Items.Add("COM1");
            // Display each port name to the console.
            foreach (string port in ports)
            {
                comboPortName.Items.Add(port);
            }

        }
        private void comboPortName_DropDown(object sender, EventArgs e)
        {
            //when the select comport is pressed, will add all available comports
            label_error.Text = "";
            InitPortName();
        }
        private void comboPortName_SelectedIndexChanged(object sender, EventArgs e)
        {
            //when a comport is selected, save the choice
            try
            {
                Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                // Get a list of serial port names.
                config.AppSettings.Settings["portname"].Value = this.comboPortName.GetItemText(this.comboPortName.SelectedItem);
                // Get the application configuration file.
                config.Save(ConfigurationSaveMode.Modified);
                ConfigurationManager.RefreshSection(config.AppSettings.SectionInformation.Name);
            }
            catch (ConfigurationErrorsException ex)
            {
                label_error.Text = ex.Message;
            }
        }
        private void OpenSerialPort()
        {
            //called when measure button is pressed and every second
            string oldport = serialPort.PortName;
            int selindex = comboPortName.SelectedIndex;

            if (comboPortName.SelectedIndex != -1) //if no port is currently selected
            {
                //set serial port to the selected port
                string PortName = this.comboPortName.GetItemText(this.comboPortName.SelectedItem);
                try
                {
                    if (serialPort.PortName != PortName)
                    {
                        if (serialPort.IsOpen)
                        {
                            serialPort.Close();
                        }
                        serialPort.PortName = PortName;

                    }
                    if (!serialPort.IsOpen)
                        serialPort.Open();
                }
                catch (Exception ex)
                {
                    SetError(ex.Message);
                    serialPort.PortName = oldport;
                    comboPortName.SelectedIndex = -1;
                }
            }
            else
            {
                SetError("Please select port.");
            }
        }

        private void DataReceivedHandler( //called whenever there is incoming serial communication
                object sender,
                SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            //if incoming message is empty, return
            if (sp.BytesToRead==0)
            {
                return;
            }
            int returnvalue=0;
            try
            {
                //create a base type telegram object
                Telegram telegram;
                telegram = new Telegram();
                //read the first incoming byte which is ID
                byte type = (byte)sp.ReadByte();
                //choosing correct handler based on type
                if (Enum.IsDefined(typeof(Telegram.telegram_type), type))
                {
                    switch ((Telegram.telegram_type)type)
                    {
                        case Telegram.telegram_type.graphdata:
                            telegram = new GraphDataTelegram();
                            break;
                        case Telegram.telegram_type.miscdata:
                            telegram = new MiscDataTelegram();
                            this.Invoke(new Action(() => this.SetError("Measurement Done")));
                            break;
                        case Telegram.telegram_type.lock_shaft:
                            string message = "Please unblock the motorshaft. Press Ok if done, or cancel to stop the measurement.";
                            string title = "Info Window";
                            MessageBoxButtons buttons = MessageBoxButtons.OKCancel;
                            DialogResult result = MessageBox.Show(message, title, buttons);
                            if (result == DialogResult.OK)
                            {
                                //when button is pressed and a serial connection is available
                                // transmit "3" which means start measurement
                                int MyInt = (int)Telegram.telegram_type.user_interaction_done;
                                byte[] b = BitConverter.GetBytes(MyInt);
                                if (this.InvokeRequired)
                                {
                                    this.Invoke(new Action(() => this.serialPort.Write(b, 0, 1)));

                                }

                            }
                            else
                            {
                                int MyInt = (int)Telegram.telegram_type.stopeverything;
                                byte[] b = BitConverter.GetBytes(MyInt);
                                if (this.InvokeRequired)
                                {
                                    this.Invoke(new Action(() => this.serialPort.Write(b, 0, 1)));

                                }
                            }
                            
                            //else label1.Text = "Received 5";
                            break;
                        case Telegram.telegram_type.measuring_KT:
                            
                            this.Invoke(new Action(() => this.SetError("Measuring KT")));
                            break;
                        case Telegram.telegram_type.measuring_ripple:
                            this.Invoke(new Action(() => this.SetError("Measuring Ripple")));
                            break;
                    }
                    //read rest of telegram
                    returnvalue=telegram.Read(sp);
                    if(type==1) //if graph call plotting
                    {
                        if (this.InvokeRequired)
                        {
                            this.Invoke(new Action(() => this.doGraphs((GraphDataTelegram)telegram)));
                        }
                        else doGraphs((GraphDataTelegram)telegram);
                    }
                    if (type == 2) //if constant write data
                    {
                        if (this.InvokeRequired)
                        {
                            this.Invoke(new Action(() => this.SetMiscLabels((MiscDataTelegram)telegram)));
                        }
                        else SetMiscLabels((MiscDataTelegram)telegram);
                    }

                }

                    //sp.Close();

            }
            catch (Exception ex)
            {
                sp.Close();
                this.Invoke(new Action(() => this.SetError(ex.Message)));

                //this.comboPortName.SelectedItem = null;
            }
        }

        private void initGraphsTS()
        {

            //initialize all the graph settings for each graph and plot
            var objChart = chartTS.ChartAreas[0];
            objChart.AxisX.IntervalType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Number;
            objChart.AxisX.Minimum = 0;
            objChart.AxisX.Maximum = 180;
            objChart.AxisY.IntervalType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Number;
            objChart.AxisY.Minimum = 0;
            objChart.AxisY.Maximum = 4000;
            objChart.AxisY2.Title = "Electrical [A & V]";
            objChart.AxisY2.Minimum = 0;
            objChart.AxisY2.Maximum = 30;
            objChart.AxisY2.TitleFont = new System.Drawing.Font("Verdana", 11, System.Drawing.FontStyle.Bold);
            objChart.Axes[0].MajorGrid.Enabled = false;
            objChart.Axes[1].MajorGrid.Enabled = true;//y axis
            objChart.AxisX.Title = "Torque [Ncm]";
            objChart.AxisY.Title = "Speed [RPM]";
            objChart.AxisX.TitleFont = new System.Drawing.Font("Verdana", 11, System.Drawing.FontStyle.Bold);
            objChart.AxisY.TitleFont = new System.Drawing.Font("Verdana", 11, System.Drawing.FontStyle.Bold);
            objChart.AxisX.Interval = 3;
            chartTS.Series.Clear();

            chartTS.Series.Add(m_TS);
            chartTS.Series[m_TS].Legend = "Legend1";
            chartTS.Series[m_TS].IsVisibleInLegend = true;
            chartTS.Series[m_TS].ChartArea = "ChartArea1";
            chartTS.Series[m_TS].ToolTip = "(Torque, Speed) = (#VALX{G}[Ncm],#VALY{G}[RPM])";
            chartTS.Series[m_TS].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chartTS.Series[m_TS].Color = Color.Blue;
            chartTS.Series[m_TS].YAxisType=System.Windows.Forms.DataVisualization.Charting.AxisType.Primary;


            chartTS.Series.Add(m_C);
            chartTS.Series[m_C].ToolTip = "(Torque, Current) = (#VALX{G}[Ncm],#VALY{G}[A])";
            chartTS.Series[m_C].Color = Color.Green;
            chartTS.Series[m_C].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chartTS.Series[m_C].YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;

            chartTS.Series.Add(m_V);
            chartTS.Series[m_V].ToolTip = "(Torque, Voltage) = (#VALX{G}[Ncm],#VALY{G}[V])";
            chartTS.Series[m_V].Color = Color.Yellow;
            chartTS.Series[m_V].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chartTS.Series[m_V].YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;

            chartTS.Series.Add("Measured");
            chartTS.Series["Measured"].Color = Color.Red;
            chartTS.Series["Measured"].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
        }
        private void initGraphsEFF()
        {
            //initialize all the graph settings for each graph and plot
            var objChartEFF = chartEFF.ChartAreas[0];
            objChartEFF.AxisX.IntervalType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Number;
            objChartEFF.AxisX.Minimum = 0;
            objChartEFF.AxisX.Maximum = 180;
            objChartEFF.AxisY.IntervalType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Number;
            objChartEFF.AxisY.Minimum = 0;
            objChartEFF.AxisY.Maximum = 100;

            objChartEFF.AxisY2.Title = "Power [W]";
            objChartEFF.AxisY2.Minimum = 0;
            objChartEFF.AxisY2.Maximum = 400;
            objChartEFF.AxisY2.TitleFont = new System.Drawing.Font("Verdana", 11, System.Drawing.FontStyle.Bold);
            objChartEFF.Axes[0].MajorGrid.Enabled = false;
            objChartEFF.Axes[1].MajorGrid.Enabled = true;//y axis
            objChartEFF.AxisX.Title = "Torque [Ncm]";
            objChartEFF.AxisY.Title = "Efficiency [%]";
            objChartEFF.AxisX.TitleFont = new System.Drawing.Font("Verdana", 11, System.Drawing.FontStyle.Bold);
            objChartEFF.AxisY.TitleFont = new System.Drawing.Font("Verdana", 11, System.Drawing.FontStyle.Bold);
            objChartEFF.AxisX.Interval = 3;
            chartEFF.Series.Clear();
            chartEFF.Series.Add(EFF);
            chartEFF.Series[EFF].Legend = "Legend1";
            chartEFF.Series[EFF].ChartArea = "ChartArea1";
            chartEFF.Series[EFF].ToolTip = "(Torque, Efficiency) = (#VALX{G}[Ncm],#VALY{G}[%])";
            chartEFF.Series[EFF].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;

            chartEFF.Series.Add(m_MW);

            chartEFF.Series[m_MW].ChartArea = "ChartArea1";
            chartEFF.Series[m_MW].ToolTip = "(Torque, Power) = (#VALX{G}[Ncm],#VALY{G}[W])";
            chartEFF.Series[m_MW].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chartEFF.Series[m_MW].Color = Color.Green;
            chartEFF.Series[m_MW].YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;

            chartEFF.Series.Add(m_EW);

            chartEFF.Series[m_EW].ChartArea = "ChartArea1";
            chartEFF.Series[m_EW].ToolTip = "(Torque, Power) = (#VALX{G}[Ncm],#VALY{G}[W])";
            chartEFF.Series[m_EW].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            chartEFF.Series[m_EW].Color = Color.Red;
            chartEFF.Series[m_EW].YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;
        }

        private void doGraphs(GraphDataTelegram measurements)
        {
            const double multfactor = 1000000.0;
            try
            {
                initGraphsTS();
                initGraphsEFF();
                //define a struct with one of each kind of value to plot
                GraphDatastruct gds = new GraphDatastruct();
                //get the first measurement set
                measurements.get_data(0, ref gds);
                //convert to double
                double s1 = gds.Speed / multfactor;
                double t1 = gds.Torque / multfactor;
                //get the last measurement set
                measurements.get_data(84, ref gds);
                double s2 = gds.Speed / multfactor;
                double t2 = gds.Torque / multfactor;
                //calculate linear function
                double a =(s2-s1)/(t2-t1);
                double b = s1 - a * t1;
                //plot no load point
                chartTS.Series[m_TS].Points.AddXY(0, b);
                //plot all graphs
                for (int i = 0; i < 86; i++)
                {
                    if (measurements.get_data(i, ref gds))
                    {
                        //calculate efficiency for each step
                        double mekpower, elecpower, torque, speed;
                        torque = (gds.Torque / (multfactor * 100.0));
                        speed = (gds.Speed / multfactor) * 2 * Math.PI / 60.0;
                        mekpower = torque * speed;
                        elecpower = ((gds.Voltage / multfactor) * (gds.Current / multfactor));
                        if(elecpower<=0)
                        {
                            throw new Exception("Electrical power was 0 or less");
                        }
                        chartEFF.Series[m_MW].Points.AddXY(gds.Torque / (multfactor), mekpower);
                        chartEFF.Series[m_EW].Points.AddXY(gds.Torque / (multfactor), elecpower);
                        chartTS.Series[m_TS].Points.AddXY(gds.Torque/(multfactor), gds.Speed/ (multfactor));
                        chartEFF.Series[EFF].Points.AddXY(gds.Torque / multfactor, mekpower/elecpower*100);
                        chartTS.Series[m_C].Points.AddXY(gds.Torque / multfactor, (gds.Current / multfactor));
                        chartTS.Series[m_V].Points.AddXY(gds.Torque / multfactor, (gds.Voltage / multfactor));
                        
                    }
                    //color the measured points for torque/speed curve red
                    if (i > 1 && i <= 85)
                    {
                        chartTS.Series[m_TS].Points[i].Color = Color.Red;
                    }
                }


                chartTS.Series[m_TS].Points.AddXY(-b/a, 0);
                Btn_Start.Text = "Measure";
                button1.Enabled = true;

            }
            catch (Exception ex)
            {
                SetError(ex.Message);
            }
            
        }
        private void SetMiscLabels(MiscDataTelegram MiscClass)
        {
            //print all the measured constants
            MiscDatastruct constants= new MiscDatastruct();
            MiscClass.get_data(ref constants);
            label_inductance.Text = ((double)constants.Inductance / 100000.0).ToString();
            label_resistance.Text = ((double)constants.Resistance / 100000.0).ToString();
            label_kt.Text = ((double)constants.Kt / 100000.0).ToString();
            label_TP.Text = ((double)constants.TRipple / 100000.0).ToString();
            button1.Text = "Measure";
            Btn_Start.Enabled = true;
        }
        private void SetError(string err)
        {
            label_error.ReadOnly = false;
            label_error.Text = err;
            label_error.ForeColor = Color.Red;
//            label_error.ReadOnly = true;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
            }
            serialPort.Dispose();
        }

        private void label_error_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            //measure constants was pressed
            //tell user to block the motor from rotating and wait for a user response
            string message = "Please block the motor shaft from rotating, using a screwdriver or similar. Press Ok if done, or cancel to stop the measurement.";
            string title = "Info Window";
            MessageBoxButtons buttons = MessageBoxButtons.OKCancel;
            DialogResult result = MessageBox.Show(message, title, buttons);
            //if user accepts,  transmit to start
            if (result == DialogResult.OK)
            {
                SetError("");
                OpenSerialPort();
                //when button is pressed and a serial connection is available
                // transmit "3" which means start measurement
                if (serialPort.IsOpen)
                {
                    int MyInt = (int)Telegram.telegram_type.findconstants;
                    byte[] b = BitConverter.GetBytes(MyInt);
                    serialPort.Write(b, 0, 1);
                    if (button1.Text == "Measure")
                    {
                        button1.Text = "Cancel Measurement";
                        SetError("Measuring Resistance and Inductance");
                        Btn_Start.Enabled = false;
                    }
                    else
                    {
                        button1.Text = "Measure";
                        Btn_Start.Enabled = true;
                    }

                }
            }
            else
            {
                
            }
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            //check that there is still a connection over the serial port
            this.Invoke(new Action(() => this.OpenSerialPort()));
        }
    }
}
