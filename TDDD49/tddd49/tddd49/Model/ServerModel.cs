using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using Newtonsoft.Json;
using System.Windows;
using System.Windows.Threading;

namespace tddd49.Model
{
    class ServerModel
    {
        public static ManualResetEvent allDone = new ManualResetEvent(false);
        private static int userPort;
        private  MainModel mainModel = null;
        private readonly StateObject so;

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
            // IPAddress localIP = Dns.GetHostEntry(Dns.GetHostName()).AddressList[0];
            IPAddress localIP = IPAddress.Parse("127.0.0.1");
            // IPEndPoint localEP = new IPEndPoint(localIP, port);
            try
            {
                System.Net.ServicePointManager.Expect100Continue = false;
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

                    string json = Encoding.UTF8.GetString(so.buffer);
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
                            SenderName = "Server", // Should be mainModel.Name
                            Date = DateTime.Now,
                            ReqIp = localIP.ToString(),
                            ReqPort = port
                        };

                        if (mainModel.AcceptedRequest)
                        {
                            response.RequestType = "connectionaccepted";
                            byte[] sendResponse = Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(response, Formatting.Indented));
                            stream.Write(sendResponse, 0, sendResponse.Length);
                            mainModel.Connected = true;
                            mainModel.FriendName = connectionRequest.SenderName;
                            mainModel.ConnectedTo = "Connected to " + mainModel.FriendName;
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
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
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
                    clientStream.BeginRead(so.buffer, 0, so.buffer.Length, new AsyncCallback(ReadCallback), clientStream);
                }
            }
            // TODO: Implement send disconnectrequest when disconnect is clicked, or handle if disconnectrequest is received.
            // TODO: Implement send images.
            // TODO: Change all SetProperties to new method!
            // TODO: Save chat-history to file.
            // TODO: Show when other user Disconnected from chat.
        }

        private void ReadCallback(IAsyncResult ar)
        {
            NetworkStream clientStream = (NetworkStream)ar.AsyncState;

            int read = clientStream.EndRead(ar);
            so.stringBuilder.Append(Encoding.ASCII.GetString(so.buffer, 0, read));

            while (clientStream.DataAvailable)
            {
                clientStream.BeginRead(so.buffer, 0, so.buffer.Length, new AsyncCallback(ReadCallback), clientStream);
            }

            if (so.stringBuilder.Length > 1)
            {
                string message = so.stringBuilder.ToString();
                Console.WriteLine(message);
                //Application.Current.Dispatcher.Invoke(new Action(() =>
                //{
                //    // Convert message to message class and send to ui
                //}));
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
                //        Send(message);
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
