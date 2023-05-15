namespace MotorCharactarizer
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.Btn_Start = new System.Windows.Forms.Button();
            this.chartTS = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.comboPortName = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.chartEFF = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.label_error = new System.Windows.Forms.TextBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.label3 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.label_TP = new System.Windows.Forms.Label();
            this.label_kt = new System.Windows.Forms.Label();
            this.label_inductance = new System.Windows.Forms.Label();
            this.label_resistance = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label_3 = new System.Windows.Forms.Label();
            this.label_2 = new System.Windows.Forms.Label();
            this.label_1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.chartTS)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartEFF)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // Btn_Start
            // 
            this.Btn_Start.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.Btn_Start.Location = new System.Drawing.Point(1184, 19);
            this.Btn_Start.Margin = new System.Windows.Forms.Padding(6);
            this.Btn_Start.Name = "Btn_Start";
            this.Btn_Start.Size = new System.Drawing.Size(150, 44);
            this.Btn_Start.TabIndex = 0;
            this.Btn_Start.Text = "Measure";
            this.Btn_Start.UseVisualStyleBackColor = true;
            this.Btn_Start.Click += new System.EventHandler(this.button1_Click);
            // 
            // chartTS
            // 
            chartArea1.Name = "ChartArea1";
            this.chartTS.ChartAreas.Add(chartArea1);
            this.chartTS.Dock = System.Windows.Forms.DockStyle.Fill;
            legend1.Name = "Legend1";
            this.chartTS.Legends.Add(legend1);
            this.chartTS.Location = new System.Drawing.Point(0, 0);
            this.chartTS.Margin = new System.Windows.Forms.Padding(6);
            this.chartTS.Name = "chartTS";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chartTS.Series.Add(series1);
            this.chartTS.Size = new System.Drawing.Size(1376, 304);
            this.chartTS.TabIndex = 1;
            this.chartTS.Text = "chartTS";
            // 
            // comboPortName
            // 
            this.comboPortName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboPortName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboPortName.FormattingEnabled = true;
            this.comboPortName.Location = new System.Drawing.Point(1148, 12);
            this.comboPortName.Margin = new System.Windows.Forms.Padding(6);
            this.comboPortName.Name = "comboPortName";
            this.comboPortName.Size = new System.Drawing.Size(238, 33);
            this.comboPortName.TabIndex = 3;
            this.comboPortName.DropDown += new System.EventHandler(this.comboPortName_DropDown);
            this.comboPortName.SelectedIndexChanged += new System.EventHandler(this.comboPortName_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(24, 17);
            this.label1.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(862, 25);
            this.label1.TabIndex = 4;
            this.label1.Text = "Please select a COM port and ensure everything is connected, before pressing Meas" +
    "ure:";
            // 
            // chartEFF
            // 
            chartArea2.Name = "ChartArea1";
            this.chartEFF.ChartAreas.Add(chartArea2);
            this.chartEFF.Dock = System.Windows.Forms.DockStyle.Fill;
            legend2.Name = "Legend1";
            this.chartEFF.Legends.Add(legend2);
            this.chartEFF.Location = new System.Drawing.Point(0, 0);
            this.chartEFF.Margin = new System.Windows.Forms.Padding(6);
            this.chartEFF.Name = "chartEFF";
            series2.ChartArea = "ChartArea1";
            series2.Legend = "Legend1";
            series2.Name = "Series1";
            this.chartEFF.Series.Add(series2);
            this.chartEFF.Size = new System.Drawing.Size(1376, 271);
            this.chartEFF.TabIndex = 6;
            this.chartEFF.Text = "chartEFF";
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 115200;
            this.serialPort.PortName = "COM3";
            this.serialPort.ReadTimeout = 500;
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.DataReceivedHandler);
            // 
            // label_error
            // 
            this.label_error.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label_error.BackColor = System.Drawing.SystemColors.Highlight;
            this.label_error.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_error.ForeColor = System.Drawing.Color.Red;
            this.label_error.Location = new System.Drawing.Point(-14, 783);
            this.label_error.Margin = new System.Windows.Forms.Padding(6);
            this.label_error.Name = "label_error";
            this.label_error.Size = new System.Drawing.Size(1440, 26);
            this.label_error.TabIndex = 7;
            this.label_error.Text = "-";
            this.label_error.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.label_error.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.label_error_KeyPress);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.Location = new System.Drawing.Point(6, 75);
            this.splitContainer1.Margin = new System.Windows.Forms.Padding(6);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.chartTS);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.chartEFF);
            this.splitContainer1.Size = new System.Drawing.Size(1376, 583);
            this.splitContainer1.SplitterDistance = 304;
            this.splitContainer1.SplitterWidth = 8;
            this.splitContainer1.TabIndex = 8;
            // 
            // tabControl1
            // 
            this.tabControl1.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(6, 63);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(6);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1404, 708);
            this.tabControl1.TabIndex = 9;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.splitContainer1);
            this.tabPage1.Controls.Add(this.Btn_Start);
            this.tabPage1.Location = new System.Drawing.Point(4, 4);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(6);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(6);
            this.tabPage1.Size = new System.Drawing.Size(1396, 670);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Graph";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(66, 29);
            this.label3.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(830, 25);
            this.label3.TabIndex = 9;
            this.label3.Text = "To generate a Torque/ Speed graph and a Torque/ Efficiency Graph, Press \"Measure\"" +
    "";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label_TP);
            this.tabPage2.Controls.Add(this.label_kt);
            this.tabPage2.Controls.Add(this.label_inductance);
            this.tabPage2.Controls.Add(this.label_resistance);
            this.tabPage2.Controls.Add(this.label7);
            this.tabPage2.Controls.Add(this.label_3);
            this.tabPage2.Controls.Add(this.label_2);
            this.tabPage2.Controls.Add(this.label_1);
            this.tabPage2.Controls.Add(this.button1);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Location = new System.Drawing.Point(4, 4);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(6);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(6);
            this.tabPage2.Size = new System.Drawing.Size(1396, 670);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Constants";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // label_TP
            // 
            this.label_TP.AutoSize = true;
            this.label_TP.Location = new System.Drawing.Point(381, 304);
            this.label_TP.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label_TP.Name = "label_TP";
            this.label_TP.Size = new System.Drawing.Size(181, 25);
            this.label_TP.TabIndex = 9;
            this.label_TP.Text = "Not yet measured";
            // 
            // label_kt
            // 
            this.label_kt.AutoSize = true;
            this.label_kt.Location = new System.Drawing.Point(381, 238);
            this.label_kt.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label_kt.Name = "label_kt";
            this.label_kt.Size = new System.Drawing.Size(181, 25);
            this.label_kt.TabIndex = 8;
            this.label_kt.Text = "Not yet measured";
            // 
            // label_inductance
            // 
            this.label_inductance.AutoSize = true;
            this.label_inductance.Location = new System.Drawing.Point(381, 167);
            this.label_inductance.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label_inductance.Name = "label_inductance";
            this.label_inductance.Size = new System.Drawing.Size(181, 25);
            this.label_inductance.TabIndex = 7;
            this.label_inductance.Text = "Not yet measured";
            // 
            // label_resistance
            // 
            this.label_resistance.AutoSize = true;
            this.label_resistance.Location = new System.Drawing.Point(381, 96);
            this.label_resistance.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label_resistance.Name = "label_resistance";
            this.label_resistance.Size = new System.Drawing.Size(181, 25);
            this.label_resistance.TabIndex = 6;
            this.label_resistance.Text = "Not yet measured";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 304);
            this.label7.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(357, 25);
            this.label7.TabIndex = 5;
            this.label7.Text = "Torque Ripple(Peak To Peak)[Ncm]:";
            // 
            // label_3
            // 
            this.label_3.AutoSize = true;
            this.label_3.Location = new System.Drawing.Point(88, 238);
            this.label_3.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label_3.Name = "label_3";
            this.label_3.Size = new System.Drawing.Size(281, 25);
            this.label_3.TabIndex = 4;
            this.label_3.Text = "Torque Constant KT [Nm/A]:";
            // 
            // label_2
            // 
            this.label_2.AutoSize = true;
            this.label_2.Location = new System.Drawing.Point(202, 167);
            this.label_2.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label_2.Name = "label_2";
            this.label_2.Size = new System.Drawing.Size(167, 25);
            this.label_2.TabIndex = 3;
            this.label_2.Text = "Inductance[mH]:";
            // 
            // label_1
            // 
            this.label_1.AutoSize = true;
            this.label_1.Location = new System.Drawing.Point(156, 96);
            this.label_1.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label_1.Name = "label_1";
            this.label_1.Size = new System.Drawing.Size(213, 25);
            this.label_1.TabIndex = 2;
            this.label_1.Text = "Resistance [Ohm(s)]:";
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(1204, 12);
            this.button1.Margin = new System.Windows.Forms.Padding(6);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(150, 44);
            this.button1.TabIndex = 1;
            this.button1.Text = "Measure";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(58, 21);
            this.label2.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(649, 25);
            this.label2.TabIndex = 0;
            this.label2.Text = "To measure the following constants for the motor, Press \"Measure\"";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 1000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1414, 827);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.label_error);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboPortName);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(6);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Motor Charactarizer";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.chartTS)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartEFF)).EndInit();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button Btn_Start;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartTS;
        private System.Windows.Forms.ComboBox comboPortName;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartEFF;
        private System.Windows.Forms.TextBox label_error;
        private System.Windows.Forms.SplitContainer splitContainer1;
        public System.IO.Ports.SerialPort serialPort;
        public System.Windows.Forms.Label label1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label_TP;
        private System.Windows.Forms.Label label_kt;
        private System.Windows.Forms.Label label_inductance;
        private System.Windows.Forms.Label label_resistance;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label_3;
        private System.Windows.Forms.Label label_2;
        private System.Windows.Forms.Label label_1;
        private System.Windows.Forms.Timer timer1;
    }
}

