using System;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using Newtonsoft.Json;
using System.Windows;
using System.Windows.Threading;
using tddd49.Viewmodel;
using System.Media;
using System.IO;
using System.Drawing;

namespace tddd49.Model
{
    class ServerModel
    {
        public static ManualResetEvent allDone = new ManualResetEvent(false);
        private static int userPort;
        private static IPAddress userAddress;
        private  MainModel mainModel = null;

        public StateObject so { get; set; }

        private bool buzzReceived = false;

        private JsonStorage jsonStorer;
        private readonly int BufferSize = 1024;


        public ServerModel(MainModel mainModel)
        {
            this.mainModel = mainModel;
            so = new StateObject();
        }

        public void StartListening(int port)
        {
            TcpListener server = null;
            TcpClient client = null;
            NetworkStream stream = null;
            IPAddress localIP = IPAddress.Parse("127.0.0.1");
            userAddress = localIP;
            userPort = port;
            try
            {
                server = new TcpListener(localIP, port);
                server.Start();

                while (true)
                {
                    Console.WriteLine("Waiting for connection....");
                    client = server.AcceptTcpClient();
                    Console.WriteLine("Connection established!");
                    stream = client.GetStream();
                    so.workStream = stream;

                    stream.Read(so.buffer, 0, so.buffer.Length);

                    string json = Encoding.ASCII.GetString(so.buffer);
                    Array.Clear(so.buffer, 0, so.buffer.Length);
                    CustomProtocol connectionRequest = JsonConvert.DeserializeObject<CustomProtocol>(json);

                    if (connectionRequest.RequestType == "establishconnection")
                    {
                   
                        mainModel.PendingInvite = true;
                        mainModel.FriendName = connectionRequest.SenderName;
                        mainModel.Invite = mainModel.FriendName + " is sending you an invite from " + connectionRequest.ReqIp;

                        while (mainModel.PendingInvite)
                        {
                            Thread.Sleep(500);
                        }
                        

                        CustomProtocol response = new CustomProtocol
                        {
                            RequestType = "noresponse",
                            SenderName = mainModel.Name,
                            Date = DateTime.Now,
                            ReqIp = localIP.ToString(),
                            ReqPort = port
                        };

                        if (mainModel.AcceptedRequest)
                        {
                            jsonStorer = new JsonStorage(mainModel.FriendName, true);
                            mainModel.Connected = true;
                            Task startChat = Task.Run(() =>
                            {
                                response.RequestType = "connectionaccepted";
                                byte[] sendResponse = Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(response, Formatting.Indented));
                                stream.Write(sendResponse, 0, sendResponse.Length);
                                mainModel.FriendName = connectionRequest.SenderName;
                                mainModel.ConnectedTo = "Connected to " + mainModel.FriendName;
                            });
                            ChatMode();
                        }
                        else
                        {
                            response.RequestType = "connectionrefused";
                            byte[] sendResponse = Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(response, Formatting.Indented));
                            stream.Write(sendResponse, 0, sendResponse.Length);
                        }
                    }
                    stream.Close();
                    client.Close();
                }
            }
            catch (ThreadAbortException e)
            {
                Thread.CurrentThread.Abort();
            }
            catch (InvalidInfoException)
            {
                throw;
            }
            catch (Exception e)
            {
                throw new InvalidInfoException("Something went wrong in the listening thread!", e);
            }
            finally
            {
                server.Stop();
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
            catch (Exception e)
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
