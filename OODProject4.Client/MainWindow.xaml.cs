/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI based remote client                    //
// ver 1.0														   //
//Author:	Shrenik Daga, Syracuse Unicersity					   //
//Source:	Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018//
/////////////////////////////////////////////////////////////////////
/*
* The Client is equipped with the capabilities of selecting, creating and sending TestRequests
*  to the mother process, i,e the TestHarness.
* The Client uses WPF to create its UI. 
* Client uses the Translator to work as the Comm which enables messge passing
*  between Client and MotherProcess
* Client is also running a seperate thread to receive messages back from the Child Processes.
*
* Depends on:
* MsgPassingCommunication
* 
*
* No changes made after initial release
* Initial release on Dec 4 2018
*/

using System;
using System.Collections.Generic;
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
using System.IO;
using System.Threading;
using MsgPassingCommunication;



namespace OODProject4.Client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public string path { get; set; }
        public string resultFixedPath { get; set; }
        public string resPath { get; set; }
        List<string> selectedFiles { get; set; } = new List<string>();

        public MainWindow()
        {
            InitializeComponent();
        }

        private Translater translater;
        private CsEndPoint endPoint, serverEndPoint;
        private Thread receiveMessages;
        private string[] cmdArgs;

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            endPoint = new CsEndPoint();
            endPoint.machineAddress = "localhost";
            cmdArgs = Environment.GetCommandLineArgs();
            endPoint.port = Int32.Parse(cmdArgs[1]);

            translater = new Translater();
            translater.listen(endPoint);

            serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8082;

            path = Directory.GetCurrentDirectory();
            path = getAncestorPath(3, path);
            path += "\\TestLibraries";
            LoadNavTab(path);

            resPath = Directory.GetCurrentDirectory();
            resPath = getAncestorPath(3, resPath);
            LoadResTab(resPath);

            resultFixedPath = Directory.GetCurrentDirectory();
            resultFixedPath = getAncestorPath(3, resultFixedPath);
            resultFixedPath += "\\TestLibraries";
            
            ReceiveMessages();

            if (cmdArgs[1] == "8081")
            {
                ReadMePopUp();
                AutomateDemo();
            }
            
               
        }

        private void SendMessages(string dll)
        {

            CsMessage msg = new CsMessage();
            msg.add("to",CsEndPoint.toString(serverEndPoint));
            msg.add("from",CsEndPoint.toString(endPoint));
            msg.add("command","sendingFile");
            msg.add("name","sendFile");
            msg.add("value", "sendingFile");
            msg.add("sendingFile",dll);
            translater.postMessage(msg);
        }

        private void ReceiveMessages()
        {
            int i = 0;
            ThreadStart threadInstance = () =>
            {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    Console.WriteLine("\nMSG: " + msg.ToString());
                    if (msg.attributes.ContainsKey("command"))
                    {
                        if (msg.value("command") == "sendingFile")
                        {
                            Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal,new Action (() => 
                            LoadResTab(resPath)));
                        }
                    }
                }
            };
            receiveMessages = new Thread(threadInstance);
            receiveMessages.IsBackground = true;
            receiveMessages.Start();
        }

        //----< find parent paths >--------------------------------------

        string getAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }
        //----< file Find Libs tab with subdirectories and files >-------

        void LoadNavTab(string path)
        {
            Dirs.Items.Clear();
            DirectoryPath.Text = path;
            FilePath.Text = path;
            string[] dirs = Directory.GetDirectories(path);
            Dirs.Items.Add("..");
            foreach (string dir in dirs)
            {
                DirectoryInfo di = new DirectoryInfo(dir);
                string name = System.IO.Path.GetDirectoryName(dir);
                Dirs.Items.Add(di.Name);
            }
            Files.Items.Clear();
            string[] files = Directory.GetFiles(path,"*.dll");
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                Files.Items.Add(name);
            }
        }

        void LoadResTab(string path)
        {
            string resultPath = path + "\\TestLogsClient";
            ResultPath.Text = resultPath;
            TestLogsClient.Items.Clear();
            
            if (Directory.GetFiles(resultPath)!= null)
            {
                string[] files = Directory.GetFiles(resultPath);
                foreach (string file in files)
                {
                    string name = System.IO.Path.GetFileName(file);
                    TestLogsClient.Items.Add(name);
                }
            }
            
    
        }

       private void LoadTestReqTab(object sender, MouseButtonEventArgs e)
       {
            if(!Files.Items.IsEmpty)
            {
                string filePath = FilePath.Text;
                string dllName = Files.SelectedValue as string;
                string[] files = Directory.GetFiles(resultFixedPath);
                Console.WriteLine("resultpath: " + System.IO.Path.Combine(resultFixedPath, dllName));
                if (!files.Contains(System.IO.Path.Combine(resultFixedPath, dllName)))
                    File.Copy(System.IO.Path.Combine(filePath, dllName), System.IO.Path.Combine(resultFixedPath, dllName));
                TestRequest.Items.Add(dllName);
            }
            
       }

        private void TestRequest_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (!TestRequest.Items.IsEmpty)
            {
                if (TestRequest.SelectedValue != null)
                {
                    int dllIndex = TestRequest.SelectedIndex;
                    TestRequest.Items.RemoveAt(dllIndex);
                }
            }
            
        }
        
        private void Execute_Click(object sender, RoutedEventArgs e)
        {
            if (!TestRequest.Items.IsEmpty)
            {
                int index = 3;
                string testRequestString;
                string[] testRequest = new string[3 + TestRequest.Items.Count];
                testRequest[0] = "TR";
                testRequest[1] = "Shrenik";
                testRequest[2] = DateTime.Now.ToString();

                foreach (string item in TestRequest.Items)
                {
                    testRequest[index] = item;
                    index++;
                    SendMessages(item);
                }
                testRequestString = string.Join(", ", testRequest);
                Console.WriteLine("TestReq: " + testRequestString);

                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint));
                msg.add("command", "testReq");
                msg.attributes.Add("Content", testRequestString);
                msg.attributes.Add("FileName", "TestReq" + DateTime.Now.Hour + DateTime.Now.Minute + DateTime.Now.Second);
                translater.postMessage(msg);
                TestRequest.Items.Clear();
            }
        }

        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (Dirs.SelectedItem != null)
            {
                string selectedDir = Dirs.SelectedItem.ToString();
                if (selectedDir == "..")
                    path = getAncestorPath(1, path);
                else
                    path = System.IO.Path.Combine(path, selectedDir);
                LoadNavTab(path);
            }
            
        }
        //----< shut down the SelectionWindow if open >------------------

        //----< show selected file in code popup window >----------------

        private void localFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string fileName = TestLogsClient.SelectedValue as string;
            string resPath1 = Directory.GetCurrentDirectory();
            resPath1 = getAncestorPath(3, resPath1);
            resPath1 += "\\TestLogsClient";
            try
            {
                string path = System.IO.Path.Combine(resPath1, fileName);
                string contents = File.ReadAllText(path);
                OODProject4.Client.ResultPopUp popup = new OODProject4.Client.ResultPopUp();
                popup.codeView.Text = contents;
                popup.Show();
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
            }
        }

        private void ShowResultLog()
        {
            string Path = Directory.GetCurrentDirectory();
            Path = getAncestorPath(3, Path);
            Path += "\\TestLogsClient";
            try
            {
                string[] filename = Directory.GetFiles(Path);
                string path = System.IO.Path.Combine(Path, filename[0]);
                string contents = File.ReadAllText(path);
                OODProject4.Client.ResultPopUp popup = new OODProject4.Client.ResultPopUp();
                popup.codeView.Text = contents;
                popup.Show();
                Thread.Sleep(1000);
                popup.Close();
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
            }
        }

        private void ReadMePopUp()
        {
            string popUpPath = Directory.GetCurrentDirectory();
            popUpPath = getAncestorPath(3,popUpPath);
            popUpPath = System.IO.Path.Combine(popUpPath,"ReadMe.txt");
            try
            {
                string contents = File.ReadAllText(popUpPath);
                OODProject4.Client.ResultPopUp popUp = new OODProject4.Client.ResultPopUp();
                popUp.codeView.Text = contents;
                popUp.Show();
                Thread.Sleep(1000);
                popUp.Close();
            }
            catch(Exception ex)
            {
                string msg = ex.Message;
            }
        }

        private void AutomateDemo()
        {
            Files.SelectedItem = Files.Items.GetItemAt(0);
            MouseButtonEventArgs args = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left) { RoutedEvent = Control.MouseDoubleClickEvent };
            Files.RaiseEvent(args);

            Execute.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));

            Thread.Sleep(3000);
            ShowResultLog();
        }
    }
}
