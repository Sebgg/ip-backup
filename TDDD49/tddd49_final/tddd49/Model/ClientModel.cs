using System;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using Newtonsoft.Json;
using System.Windows;
using tddd49.Viewmodel;
using System.Linq;
using System.Windows.Threading;
using System.Media;
using System.IO;
using System.Drawing;

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
        private JsonStorage jsonStorer;

        public StateObject so { get; set; }

        private readonly int userPort = 12345;
        private readonly int BufferSize = 1024;
        private IPAddress userAddress;
        private bool buzzReceived = false;

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

                    IPAddress[] ipList = Dns.GetHostEntry(Dns.GetHostName()).AddressList;
                    userAddress = ipList.Where(x => x.AddressFamily == AddressFamily.InterNetwork).FirstOrDefault();

                    CustomProtocol connRequest = new CustomProtocol
                    {
                        RequestType = "establishconnection",
                        SenderName = mainModel.Name,
                        Date = DateTime.Now,
                        ReqIp = userAddress.ToString(),
                        ReqPort = userPort
                    };

                    Console.WriteLine("Name = " + connRequest.SenderName);

                    string json = JsonConvert.SerializeObject(connRequest, Formatting.Indented);
                    byte[] connectionRequest = Encoding.UTF8.GetBytes(json);

                    stream.Write(connectionRequest, 0, connectionRequest.Length);
                    stream.Read(so.buffer, 0, so.buffer.Length);

                    CustomProtocol connectionResponse = JsonConvert.DeserializeObject<CustomProtocol>(Encoding.UTF8.GetString(so.buffer));
                    Array.Clear(so.buffer, 0, so.buffer.Length);
                    if (connectionResponse.RequestType == "connectionaccepted")
                    {
                        jsonStorer = new JsonStorage(connectionResponse.SenderName, false);
                        mainModel.ListenVisible = false;
                        mainModel.FriendName = connectionResponse.SenderName;
                        mainModel.ConnectedTo = "Connected to " + mainModel.FriendName; 
                        mainModel.Connected = true;
                        ChatMode();
                    }
                    else
                    {
                        throw new InvalidInfoException("Target host actively refused connection.");
                    }
                }
                else
                {
                    throw new InvalidInfoException("The other end is busy, or not active!");
                }

                connectDone.WaitOne();

                try
                {
                    stream.Close();
                    client.Close();
                }
                catch (Exception e)
                {
                    throw new InvalidInfoException("Could not close connection properly!", e);
                }
            }
            catch (InvalidInfoException)
            {
                throw;
            }
            catch (Exception e)
            {
                throw new InvalidInfoException("Something went wrong in the connection!", e);
            }
        }

        private void ChatMode()
        {
            NetworkStream clientStream = so.workStream;

            while (mainModel.Connected)
            {
                if (clientStream.DataAvailable)
                {
                    ReadStream();
                }

                if (buzzReceived)
                {
                    SystemSounds.Beep.Play();
                    buzzReceived = false;
                }
            }
            if (!mainModel.DisconnectReceived)
            {
                CustomProtocol disconnect = new CustomProtocol
                {
                    RequestType = "closeconnection",
                    SenderName = mainModel.Name,
                    Date = DateTime.Now,
                    ReqIp = userAddress.ToString(),
                    ReqPort = userPort
                };
                byte[] messageType = Encoding.UTF8.GetBytes("ptcl");
                clientStream.Write(messageType, 0, messageType.Length);
                byte[] disconnectRequest = Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(disconnect, Formatting.Indented));
                clientStream.Write(disconnectRequest, 0, disconnectRequest.Length);
            }
            mainModel.DisconnectReceived = false;
        }

        private void ReadStream()
        {
            NetworkStream clientStream = so.workStream;
            byte[] buffer = new byte[BufferSize];
            int numberOfBytesRead = 0;
            StringBuilder stringBuilder = new StringBuilder();
            byte[] typeBuffer = new byte[4];

            clientStream.Read(typeBuffer, 0, 4);
            string messageType = Encoding.UTF8.GetString(typeBuffer);

            if (messageType == "text")
            {
                do
                {
                    numberOfBytesRead = clientStream.Read(buffer, 0, buffer.Length);

                    stringBuilder.Append(Encoding.UTF8.GetString(buffer), 0, numberOfBytesRead);
                }
                while (clientStream.DataAvailable);

                if (stringBuilder.Length >= 1)
                {
                    string message = stringBuilder.ToString();
                    stringBuilder.Clear();
                    string messagecopy = message;

                    TextMessage toUi = new TextMessage(mainModel.FriendName, message);
                    Application.Current.Dispatcher.Invoke(new Action(() =>
                    {
                        mainModel.Messages.AddMessage(toUi);
                    }), DispatcherPriority.DataBind);
                    ThreadPool.QueueUserWorkItem(state => jsonStorer.addMessage(toUi));
                    mainModel.DisconnectReceived = false;
                }
            }
            else if (messageType == "bild")
            {
                DateTime time = DateTime.Now;
                string date = time.Year.ToString() + time.Month.ToString() + time.Day.ToString() + time.Hour.ToString() + time.Minute.ToString() + time.Second.ToString();
                string fileName = mainModel.FriendName + "_" + date + ".png";
                
                string fullPath = Path.GetFullPath(@"History");
                string filePath = Path.Combine(fullPath, fileName);

                using (MemoryStream ms = new MemoryStream())
                {
                    do
                    {
                        numberOfBytesRead = clientStream.Read(buffer, 0, buffer.Length);
                        ms.Write(buffer, 0, numberOfBytesRead);
                    }
                    while (clientStream.DataAvailable);

                    Bitmap image = new Bitmap(ms);
                    image.Save(filePath);
                }

                Uri uri = new Uri(filePath);
                ImageMessage imgMessage = new ImageMessage(mainModel.FriendName, uri);

                Application.Current.Dispatcher.Invoke(new Action(() => 
                {
                    mainModel.Messages.AddMessage(imgMessage);
                }), DispatcherPriority.DataBind);
                ThreadPool.QueueUserWorkItem(state => jsonStorer.addMessage(imgMessage));
            }
            else if (messageType == "ptcl")
            {
                clientStream.Read(buffer, 0, buffer.Length);
                CustomProtocol deserMessage = JsonConvert.DeserializeObject<CustomProtocol>(Encoding.UTF8.GetString(buffer));
                if (deserMessage.RequestType == "closeconnection")
                {
                    mainModel.DisconnectReceived = true;
                    mainModel.Disconnect();
                }
                else if (deserMessage.MessageType == "buzz")
                {
                    buzzReceived = true;
                }
            }
        }

        public void Send(string message, byte[] messageType)
        {
            NetworkStream clientStream = so.workStream;
            try
            {
                clientStream.Write(messageType, 0, messageType.Length);

                byte[] sendToFriend = Encoding.UTF8.GetBytes(message);
                clientStream.Write(sendToFriend, 0, sendToFriend.Length);

                TextMessage sentMessage = new TextMessage(mainModel.Name, message);
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    mainModel.Messages.AddMessage(sentMessage);
                }), DispatcherPriority.DataBind);
                ThreadPool.QueueUserWorkItem(state => jsonStorer.addMessage(sentMessage));
            }
            catch (Exception e)
            {
                throw new InvalidInfoException("Can't send message when not connected to peer!", e);
            } 
        }

        public void SendImage(byte[] image, byte[] messageType, Uri filePath)
        {
            NetworkStream clientStream = so.workStream;

            try
            {
                clientStream.Write(messageType, 0, messageType.Length);
                clientStream.Write(image, 0, image.Length);
                //TODO: Save image somehow to chat history
                ImageMessage sentMessage = new ImageMessage(mainModel.Name, filePath);
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    mainModel.Messages.AddMessage(sentMessage);
                }), DispatcherPriority.DataBind);
                ThreadPool.QueueUserWorkItem(state => jsonStorer.addMessage(sentMessage));
            }
            catch(Exception e) 
            {
                throw new InvalidInfoException("Can't send image when not connected to peer!", e);
            }
        }

        public void SendBuzz(byte[] buzz, byte[] messageType)
        {
            NetworkStream clientStream = so.workStream;
            try
            {
                clientStream.Write(messageType, 0, messageType.Length);
                clientStream.Write(buzz, 0, buzz.Length);
            }
            catch (Exception e)
            {

                throw new InvalidInfoException("Can't buzz when not connected to peer!", e);
            }
        }
    }
}
