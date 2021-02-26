using System;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using Newtonsoft.Json;
using System.Windows;
using tddd49.Viewmodel;



namespace tddd49.Model
{
    public class StateObject
    {
        public NetworkStream workStream = null;
        public const int BufferSize = 1024;
        public byte[] buffer = new byte[BufferSize];
        public StringBuilder stringBuilder = new StringBuilder();

    }

    class ClientModel
    {
        private static ManualResetEvent connectDone =
        new ManualResetEvent(false);
        private static ManualResetEvent sendDone =
            new ManualResetEvent(false);
        private static ManualResetEvent receiveDone =
            new ManualResetEvent(false);
        private MainModel mainModel;



        private byte[] received = new byte[256];

        private static string response = string.Empty;

        private StateObject so;

        private string filePath = "";

        public ClientModel(MainModel mainModel)
        {
            so = new StateObject();
            this.mainModel = mainModel;
        }

        public void ConnectToPeer(IPAddress otherEnd, int port)
        {
            TcpClient client = null;
            NetworkStream stream = null;
            try
            {
                client = new TcpClient("127.0.0.1", port);
                Console.WriteLine(client.GetType());
                if (client.Connected)
                {
                    stream = client.GetStream();
                    so.workStream = stream;

                    IPAddress iP = Dns.GetHostEntry(Dns.GetHostName()).AddressList[0];
                    string sendIp = iP.ToString();
                    CustomProtocol connRequest = new CustomProtocol
                    {
                        RequestType = "establishconnection",
                        SenderName = mainModel.Name,
                        Date = DateTime.Now,
                        ReqIp = sendIp,
                        ReqPort = 23273
                    };

                    Console.WriteLine("Name = " + connRequest.SenderName);

                    string json = JsonConvert.SerializeObject(connRequest, Formatting.Indented);
                    byte[] connectionRequest = Encoding.UTF8.GetBytes(json);

                    stream.Write(connectionRequest, 0, connectionRequest.Length);
                    stream.Read(so.buffer, 0, so.buffer.Length);

                    CustomProtocol connectionResponse = JsonConvert.DeserializeObject<CustomProtocol>(Encoding.UTF8.GetString(so.buffer));
                    if (connectionResponse.RequestType == "connectionaccepted")
                    {
                        mainModel.FriendName = connectionResponse.SenderName;
                        mainModel.Connected = true;
                        mainModel.ConnectedTo = "Connected to " + mainModel.FriendName; 
                        ChatMode();
                    }
                    else
                    {
                        throw new Exception("Target host actively refused connection.");
                    }
                }
                else
                {
                    throw new Exception("The other end is busy, or not active!");
                }
                connectDone.WaitOne();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.StackTrace);
                Console.WriteLine(e.Message);
                throw new Exception(e.Message, e);
            }
            finally
            {
                stream.Close();
                client.Close();
            }
            
        }

        private void ChatMode()
        {
            NetworkStream clientStream = so.workStream;

            while (mainModel.Connected)
            {
                if (clientStream.DataAvailable)
                {
                    clientStream.BeginRead(so.buffer, 0, so.buffer.Length, new AsyncCallback(ReadCallback), clientStream);
                }
            }
        }

        private void ReadCallback(IAsyncResult ar)
        {
            NetworkStream clientStream = (NetworkStream)ar.AsyncState;

            int read = clientStream.EndRead(ar);
            so.stringBuilder.Append(Encoding.ASCII.GetString(so.buffer, 0, read));

            while(clientStream.DataAvailable) {
                clientStream.BeginRead(so.buffer, 0, so.buffer.Length, new AsyncCallback(ReadCallback), clientStream);
            }

            if (so.stringBuilder.Length > 1)
            {
                string message = so.stringBuilder.ToString();
                Console.WriteLine(message);
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    TextMessage toUi = new TextMessage(mainModel.FriendName, message);
                    // Convert message to message class and send to ui
                }));
            }
        }

        public void Send(String message)
        {
            Console.WriteLine(message);
            NetworkStream clientStream = so.workStream;
            if (clientStream.CanWrite)
            {
                byte[] sendToFriend = Encoding.UTF8.GetBytes(message);
                clientStream.Write(sendToFriend, 0, sendToFriend.Length);
                //clientStream.Read(so.buffer, 0, so.buffer.Length);
                //CustomProtocol recv = JsonConvert.DeserializeObject<CustomProtocol>(Encoding.UTF8.GetString(so.buffer));
                //if (recv.RequestType != "messagereceived")
                //{
                //    if (MessageBox.Show("Message not sent, try again?", "MessageError", MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.Yes)
                //    {
                //            Send(message);
                //    }
                //}
                //else
                //{
                    Console.WriteLine("Message sent!");
                //}
            }
            else
            {
                throw new InvalidInfoException("CannotWriteException");
            }

        }
    }
}
