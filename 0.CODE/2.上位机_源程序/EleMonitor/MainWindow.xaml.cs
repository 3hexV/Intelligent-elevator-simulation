using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace EleMonitor
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public SerialPort serialPort;//串口对象类
        int flag = 0;
        public MainWindow()
        {
            InitializeComponent();
           
        }
        /// 串口接收通信配置方法
        /// 端口名称
        public bool InitCOM(string PortName)
        {
            serialPort = new SerialPort(PortName, 9600, Parity.None, 8, StopBits.One);
            serialPort.DataReceived += new SerialDataReceivedEventHandler(serialPort_DataReceived);//DataReceived事件委托
            serialPort.ReceivedBytesThreshold = 1;
            serialPort.RtsEnable = true;
            return OpenPort();//串口打开
        }
        /// 数据接收事件
        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int count = 0;
            // Thread.Sleep(2000);
            byte[] readBuffer = new byte[serialPort.ReadBufferSize];
            SetTextCallback d = new SetTextCallback(SetText);
            serialPort.Read(readBuffer, 0, readBuffer.Length);

            if (readBuffer[1] == 0x03) // 火焰
            {
                if (readBuffer[2] == 0x00)
                {
                    this.Dispatcher.Invoke(d, new object[] { "1@" + "报警" });
                    this.Dispatcher.Invoke(d, new object[] { "2@" + "亮起" });
                    MessageBox.Show("发现明火！！！");
                }
                else if (readBuffer[2] == 0x01)
                {
                    this.Dispatcher.Invoke(d, new object[] { "1@" + "正常" });
                    this.Dispatcher.Invoke(d, new object[] { "2@" + "熄灭" });
                }
            }
            else if (readBuffer[1] == 0x04) //光照
            {
                if (readBuffer[2] == 0x00)
                {
                    this.Dispatcher.Invoke(d, new object[] { "3@" + "一档" });
                    this.Dispatcher.Invoke(d, new object[] { "4@" + "亮起" });
                    this.Dispatcher.Invoke(d, new object[] { "5@" + "熄灭" });
                }
                else if (readBuffer[2] == 0x01)
                {
                    this.Dispatcher.Invoke(d, new object[] { "3@" + "二档" });
                    this.Dispatcher.Invoke(d, new object[] { "4@" + "亮起" });
                    this.Dispatcher.Invoke(d, new object[] { "5@" + "亮起" });
                }
            }

            string str = "";
            foreach (byte b in readBuffer)
            {
                count++;
                if (count > 5) break;
                str += "0x" + (b<=0x0f?"0":"")+ Convert.ToString(b, 16).ToUpper() + " ";
            
            }
            this.Dispatcher.Invoke(d, new object[] { "0@"+str });
        }
        delegate void SetTextCallback(string text);

        private void SetText(string text)
        {
            if(text[0]=='0')
                this.tb_rec.Text = text.Split('@')[1];
            else if (text[0] == '1')
                this.tb_fire_state.Text = text.Split('@')[1];
            else if (text[0] == '2')
                this.tb_fire_light.Text = text.Split('@')[1];
            else if (text[0] == '3')
                this.tb_light_state.Text = text.Split('@')[1];
            else if (text[0] == '4')
                this.tb_one_light.Text = text.Split('@')[1];
            else if (text[0] == '5')
                this.tb_two_light.Text = text.Split('@')[1];
        }
        //打开串口的方法
        public bool OpenPort()
        {
            try//这里写成异常处理的形式以免串口打不开程序崩溃
            {
                serialPort.Open();
            }
            catch { }
            if (serialPort.IsOpen)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        //关闭串口的方法
        public bool ClosePort()
        {
            try
            {
                serialPort.Close();
            }
            catch { }
            if (serialPort.IsOpen)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        //向串口发送数据
        //        public void SendCommand(string CommandString)
        //       {
        //          byte[] WriteBuffer = Encoding.ASCII.GetBytes(CommandString);
        //          serialPort.Write(WriteBuffer, 0, WriteBuffer.Length);
        //     }

        private void btn_Open_Click(object sender, RoutedEventArgs e)
        {
            string comName = this.tb_com.Text;
            if (flag == 0)
            { 
                if (InitCOM(comName))
                {
                    this.lb_state.Foreground = new SolidColorBrush(Colors.Green);
                    this.btn_Open.Content = "断开通信";
                    flag = 1;
                }
                else {
                    this.lb_state.Foreground = new SolidColorBrush(Colors.Red);
                    MessageBox.Show("串口打开失败");
                }
                
            }else if(flag==1)
            {
                if (ClosePort())
                {
                    this.lb_state.Foreground = new SolidColorBrush(Colors.Red);
                    this.btn_Open.Content = "建立通信";
                    MessageBox.Show("串口关闭成功！");
                    flag = 0;
                }
                else
                {
                    this.lb_state.Foreground = new SolidColorBrush(Colors.Green);
                }
                
            }
            
        }
    }
}
