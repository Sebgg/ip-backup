using System;
using System.Net;
using System.Threading;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Threading;
using System.Windows;
using tddd49.Viewmodel;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using System.Text;
using tddd49.Model.ExtraClasses;
using System.Linq;
using System.Windows.Media.Imaging;

namespace tddd49.Model
{
    internal class MainModel : INotifyPropertyChanged
    {

        private ClientModel clientModel = null;
        private ServerModel serverModel = null;
        private bool _pendingInvite = false;
        private bool _notConnecting = true;
        private bool _connected = false;
        private string _name;
        private string _friendName = "";
        private bool _acceptedRequest = false;
        private string _invite = "No pending invite";
        private string _recievedMessage = "";
        private bool _listening = false;
        private string _connectedTo = "";
        private bool _listenVisible = true;
        private bool _inviteVisible = false;
        private MessageList _messages;
        private List<ConversationStorage> conversations;
        private bool _disconnectReceived = false;
        //Constructor for MainModel
        public MainModel()
        {
            serverModel = new ServerModel(this);
            clientModel = new ClientModel(this);
            Messages = new MessageList();
        }

        public bool Connected
        {
            get => _connected;
            set
            {
                _connected = value;
                OnPropertyChanged();
            }
        }
        public bool NotConnecting
        {
            get => _notConnecting;
            set
            {
                _notConnecting = value;
                OnPropertyChanged();
            }
        }
        public bool PendingInvite
        {
            get => _pendingInvite;
            set
            {
                _pendingInvite = value;
                OnPropertyChanged();
            }
        }
        public string Name
        {
            get { return _name; }
            set 
            {
                _name = value;
                OnPropertyChanged();
            } 
        }
        public string FriendName
        {
            get => _friendName;
            set
            {
                _friendName = value;
                OnPropertyChanged();
            }
        }
        public bool AcceptedRequest
        {
            get => _acceptedRequest;
            set
            {
                _acceptedRequest = value;
                OnPropertyChanged();
            }
        }
        public string Invite
        {
            get => _invite;
            set
            {
                _invite = value;
                OnPropertyChanged();
            }
        }
        public string RecievedMessage
        {
            get => _recievedMessage;
            set
            {
                _recievedMessage = value;
                OnPropertyChanged();
            }
        }

        public bool Listening
        {
            get => _listening;
            set
            {
                _listening = value;
                OnPropertyChanged();
            }
        }

        public string ConnectedTo
        {
            get => _connectedTo;
            set
            {
                _connectedTo = value;
                OnPropertyChanged();
            }
        }
        public bool ListenVisible
        {
            get => _listenVisible;
            set
            {
                _listenVisible = value;
                OnPropertyChanged();
            }
        }
        public bool InviteVisible
        {
            get => _inviteVisible;
            set
            {
                _inviteVisible = value;
                OnPropertyChanged();
            }
        }

        public MessageList Messages
        {
            get => _messages;
            set
            {
                _messages = value;
                OnPropertyChanged();
            }
        }

        public bool DisconnectReceived
        {
            get => _disconnectReceived;
            set
            {
                _disconnectReceived = value;
                OnPropertyChanged();
            }
        }

        public Thread ListenThread { get; set; }

        public void Connecter(string ipAddress, int port)
        {
            IPAddress otherEnd = IPAddress.Parse(ipAddress); //ip address for computer to connect to
            try
            {
                clientModel.ConnectToPeer(otherEnd, port);
            }
            catch (InvalidInfoException)
            {
                throw;
            }
        }

        public void Listener(int prt)
        {
            try
            {
                Thread t2 = new Thread(delegate ()
                {
                    Listening = true;
                    serverModel.StartListening(prt);
                });
                ListenThread = t2;
                ListenThread.Name = "ListenerThread";
                ListenThread.IsBackground = true;
                ListenThread.Start();
            }
            catch (InvalidInfoException)
            {
                throw;
            }
            catch (Exception e)
            {
                throw new InvalidInfoException("Could not start listening!", e);
            }
        }

        public void SendMessage(string message)
        {
            string messageType = "text";
            byte[] messageProtocol = Encoding.UTF8.GetBytes(messageType);

            try
            {

                message = message.TrimEnd();
                if (Listening)
                {
                    serverModel.Send(message, messageProtocol);
                }
                else
                {
                    clientModel.Send(message, messageProtocol);
                }
            }        
            catch (InvalidInfoException)
            {
                throw;
            }
        }

        public void SendImage(BitmapImage image, Uri filePath)
        {
            try
            {
                string messageType = "bild";
                byte[] messageProtocol = Encoding.UTF8.GetBytes(messageType);

                byte[] bytesToSend;
                PngBitmapEncoder encoder = new PngBitmapEncoder();
                encoder.Frames.Add(BitmapFrame.Create(image));
                using (MemoryStream ms = new MemoryStream())
                {
                    encoder.Save(ms);
                    bytesToSend = ms.ToArray();
                }
                if (Listening)
                {
                    serverModel.SendImage(bytesToSend, messageProtocol, filePath);
                }
                else
                {
                    clientModel.SendImage(bytesToSend, messageProtocol, filePath);
                }
            }
            catch (InvalidInfoException)
            {
                throw;
            }
            catch (Exception e)
            {
                throw new InvalidInfoException("Something went wrong while sending image!", e);
            }
        }

        public void SendBuzz()
        {
            CustomProtocol bzz = new CustomProtocol
            {
                MessageType = "buzz",
                SenderName = Name,
                Date = DateTime.Now,
            };

            byte[] buzz = Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(bzz, Formatting.Indented));
            string messageType = "ptcl";
            byte[] messageProtocol = Encoding.UTF8.GetBytes(messageType);

            try 
            { 
                if (Listening)
                {
                    serverModel.SendBuzz(buzz, messageProtocol);
                }
                else
                {
                    clientModel.SendBuzz(buzz, messageProtocol);
                }
            }
            catch (InvalidInfoException)
            {
                throw;
            }
            catch (Exception e)
            {
                throw new InvalidInfoException("Something went wrong while buzzing partner!", e);
            }
        }
        public void Disconnect()
        {
            Connected = false;
            Application.Current.Dispatcher.Invoke(new Action(() =>
            {
                Messages.RemoveAll();
            }), DispatcherPriority.Background);
            if (Listening)
            {
                InviteVisible = true;
                Invite = "No pending invites";
            }
            else
            {
                ListenVisible = true;
            }
        }

        public List<Conversation> GetConversations()
        {
            List<Conversation> temp = new List<Conversation>();
            string path = @"History";
            string workPath;
            if (Listening)
            {
                workPath = Path.Combine(path, "serverconversations.json");
            }
            else
            {
                workPath = Path.Combine(path, "clientconversations.json");
            }
            try
            {
                if (!File.Exists(workPath))
                {
                    Directory.CreateDirectory(path);
                }
                var jsonData = File.ReadAllText(workPath);

                conversations = JsonConvert.DeserializeObject<List<ConversationStorage>>(jsonData) ?? new List<ConversationStorage>();
                
                foreach (ConversationStorage cs in conversations)
                {
                    Conversation tempC = new Conversation();
                    tempC.ConvName = cs.ChatWith;
                    tempC.ConvDate = cs.CreationTime;
                    tempC.ConvMessages = new MessageList();
                    foreach (Message m in cs.Conversation)
                    {
                        tempC.ConvMessages.Add(m);
                    }
                    temp.Add(tempC);
                }
            }
            catch (FileNotFoundException)
            {
                return temp;
            }
            catch (Exception e)
            {
                throw new InvalidInfoException("Something went wrong while getting conversation history!", e);
            }
            temp.Reverse();
            return temp;
        }
        
        public void ShowConversation(string searchTime)
        {
            ConversationStorage conversationStorage = conversations.Find(x => searchTime == x.CreationTime.ToString());
            MessageList list = new MessageList(conversationStorage);
            Messages = list;
        }

        public List<Conversation> FindConversations(string searchString)
        {
            IEnumerable<ConversationStorage> convQuery = from conversation in conversations
                                                         where conversation.ChatWith.ToLower().Contains(searchString.ToLower())
                                                         select conversation;

            List<Conversation> newConversations = new List<Conversation>();
            foreach (var conversation in convQuery)
            {
                Conversation tempC = new Conversation();
                tempC.ConvName = conversation.ChatWith;
                tempC.ConvDate = conversation.CreationTime;
                tempC.ConvMessages = new MessageList();
                foreach (Message m in conversation.Conversation)
                {
                    tempC.ConvMessages.Add(m);
                }
                newConversations.Add(tempC);
            }
            newConversations.Reverse();
            return newConversations;
        }

        public void SetName(string name)
        {
            Name = name;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    public class CustomProtocol
    {
        public string RequestType { get; set; } //establishconnection, declineconnection, acceptconnection, closeconnection, message
        public string SenderName { get; set; }
        public DateTime Date { get; set; }
        public string ReqIp { get; set; }
        public int ReqPort { get; set; }
        public string MessageType { get; set; }
    }
}
