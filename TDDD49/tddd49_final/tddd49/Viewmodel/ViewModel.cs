using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Net;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using tddd49.Model;
using System.Threading;
using System.Windows;
using System.Diagnostics;
using Microsoft.Win32;
using System.Windows.Media.Imaging;
using System.Text.RegularExpressions;

namespace tddd49.Viewmodel
{
    internal class ViewModel : INotifyPropertyChanged
    {
        // Strings
        private string _ipAddress = "127.0.0.1";
        private string _port = "8888";
        private string _connect = "Connect";
        private string _listenport = "8888";
        private string _message = "";
        private string _searchParameter;

        // Booleans
        private bool _listenVisible = true;
        private bool _inviteVisible = false;

        // Lists
        private List<Conversation> _conversations;

        // From Model
        // Strings
        private string _name = "";
        private string _friendName = "";
        private string _invite = "No pending invites";
        private string _connectedTo;

        // Booleans
        private bool _connected = false;
        private bool _pendingInvite = false;
        private bool _acceptedRequest = false;

        // Other
        private MessageList _messages = null;
        public MainModel mainModel;


        //Constructors for ViewModel
        public ViewModel()
        {
        }
        public ViewModel(MainModel model)
        {
            _getInfoCommand = new DelegateCommand(OnGetInfo, CanGetInfo);
            _getPortListenCommand = new DelegateCommand(OnListenPort, CanListenPort);
            _acceptInviteCommand = new DelegateCommand(OnAcceptInvite, CanAcceptInvite);
            _declineInviteCommand = new DelegateCommand(OnDeclineInvite, CanDeclineInvite);
            _sendCommand = new DelegateCommand(OnSend, CanSend);
            _disconnectCommand = new DelegateCommand(OnDisconnect, CanDisconnect);
            _showConversationCommand = new DelegateCommand(OnShowConversation, CanShowConversation);
            _searchConversationsCommand = new DelegateCommand(OnSearchConversations, CanSearchConversations);
            _buzzCommand = new DelegateCommand(OnBuzz, CanBuzz);
            _addImageCommand = new DelegateCommand(OnAddImage, CanAddImage);
            mainModel = model;
            mainModel.PropertyChanged += MainModel_PropertyChanged;

            PopulateConversations();
        }

        public string IpAddress
        {
            get => _ipAddress;
            set
            {
                _ipAddress = value;
                OnPropertyChanged();
            }
        }
        public string Port
        {
            get => _port;
            set
            {
                _port = value;
                OnPropertyChanged();
            }
        }
        public string Connect
        {
            get => _connect;
            set
            {
                _connect = value;
                OnPropertyChanged();
            }
        }
        public string ListenPort
        {
            get => _listenport;
            set
            {
                _listenport = value;
                OnPropertyChanged();
            }
        }
        public bool ListenVisible
        {
            get => mainModel.ListenVisible;
            set
            {
                _listenVisible = value;
                OnPropertyChanged();
            }
        }
        public bool InviteVisible
        {
            get => mainModel.InviteVisible;
            set
            {
                _inviteVisible = value;
                OnPropertyChanged();
            }
        }
        public string Message
        {
            get => _message;
            set
            {
                _message = value;
                OnPropertyChanged();
            }
        }

        public string SearchParameter 
        { 
            get => _searchParameter;
            set 
            {
                _searchParameter = value;
                OnPropertyChanged();
            } 
        }


        public List<Conversation> Conversations
        {
            get => _conversations;
            set
            {
                _conversations = value;
                OnPropertyChanged();
            }
        }

        //From MainModel
        public bool Connected
        {
            get { return mainModel.Connected; }
            set
            {
                _connected = value;
                OnPropertyChanged();
            }
        }

        public bool PendingInvite
        {
            get { return mainModel.PendingInvite; }
            set
            {
                _pendingInvite = value;
                OnPropertyChanged();
            }
        }
        public string Name
        {
            get => _name;
            set
            {
                _name = value;
                OnPropertyChanged();
            }
        }
        public string FriendName
        {
            get => mainModel.FriendName;
            set
            {
                _friendName = value;
                OnPropertyChanged();
            }
        }
        public bool AcceptedRequest
        {
            get => mainModel.AcceptedRequest;
            set
            {
                _acceptedRequest = value;
                OnPropertyChanged();
            }
        }
        public string Invite
        {
            get => mainModel.Invite;
            set
            {
                _invite = value;
                OnPropertyChanged();
            }
        }

        public string ConnectedTo
        {
            get => mainModel.ConnectedTo;
            set
            {
                _connectedTo = value;
                OnPropertyChanged();
            }
        }
        public MessageList Messages
        {
            get => mainModel.Messages;
            set
            {
                _messages = value;
                OnPropertyChanged();
            }
        }

        private readonly DelegateCommand _getInfoCommand;
        public ICommand GetInfoCommand => _getInfoCommand;

        private readonly DelegateCommand _getPortListenCommand;
        public ICommand GetPortListenCommand => _getPortListenCommand;

        private readonly DelegateCommand _acceptInviteCommand;
        public ICommand AcceptInviteCommand => _acceptInviteCommand;

        private readonly DelegateCommand _declineInviteCommand;
        public ICommand DeclineInviteCommand => _declineInviteCommand;

        private readonly DelegateCommand _sendCommand;
        public ICommand SendCommand => _sendCommand;

        private readonly DelegateCommand _disconnectCommand;
        public ICommand DisconnectCommand => _disconnectCommand;

        private readonly DelegateCommand _showConversationCommand;
        public ICommand ShowConversationCommand => _showConversationCommand;
        
        private readonly DelegateCommand _searchConversationsCommand;
        public ICommand SearchConversationsCommand => _searchConversationsCommand;

        private readonly DelegateCommand _buzzCommand;
        public ICommand BuzzCommand => _buzzCommand;

        private readonly DelegateCommand _addImageCommand;
        public ICommand AddImageCommand => _addImageCommand;


        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        
        public void MainModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch(e.PropertyName)
            {
                case "Connected":
                    Connected = mainModel.Connected;
                    break;
                case "PendingInvite":
                    PendingInvite = mainModel.PendingInvite;
                    break;
                case "Name":
                    Name = mainModel.Name;
                    break;
                case "FriendName":
                    FriendName = mainModel.FriendName;
                    break;
                case "AcceptedRequest":
                    AcceptedRequest = mainModel.AcceptedRequest;
                    break;
                case "Invite":
                    Invite = mainModel.Invite;
                    break;
                case "ConnectedTo":
                    ConnectedTo = mainModel.ConnectedTo;
                    break;
                case "InviteVisible":
                    InviteVisible = mainModel.InviteVisible;
                    break;
                case "ListenVisible":
                    ListenVisible = mainModel.ListenVisible;
                    break;
                case "Messages":
                    Messages = mainModel.Messages;
                    break;
                case "DisconnectReceived":
                    if (mainModel.DisconnectReceived)
                    {
                        MessageBox.Show(FriendName + " has disconnected from chat!", "Disconnect", MessageBoxButton.OK, MessageBoxImage.Information);
                    }
                    break;
                default:
                    break;
            }
        }

        private void PopulateConversations()
        {
            try
            {
                Thread convThread = new Thread(delegate ()
                {
                    while (true)
                    {
                        Conversations = mainModel.GetConversations();
                        Thread.Sleep(10000);
                    }

                });
                convThread.IsBackground = true;
                convThread.Start();
            }
            catch (Exception)
            {
                throw;
            }
        }

        private void OnGetInfo(object commandParameter)
        {
            try
            {
                if (Name.Length < 1)
                {
                    throw new InvalidInfoException("Name needs to be at least one character long");
                } 
                if (IPAddress.TryParse(IpAddress, out IPAddress _))
                {
                    if (Int32.TryParse(Port, out int prt) && (1023 < prt && prt < 49152))
                    {
                        string ipAddress = IpAddress;
                        mainModel.SetName(Name);
                        mainModel.Messages.RemoveAll();
                        Thread thread = new Thread(delegate ()
                        {
                            try
                            {
                                mainModel.Connecter(ipAddress, prt);
                            }
                            catch (Exception e)
                            {
                                MessageBox.Show(e.Message, "Connection Error", MessageBoxButton.OK, MessageBoxImage.Error);
                            }
                        });
                        thread.Name = "ConnectToPeerThread";
                        thread.IsBackground = true;
                        thread.Start();
                    }
                    else
                    {
                        throw new InvalidInfoException("Invalid portnumber, must be a number between 1023 and 49152");
                    }
                }
                else
                {
                    throw new InvalidInfoException("Invalid Ip-address, must be between 0.0.0.0 and 255.255.255.255");
                }
            }
            catch (InvalidInfoException e)
            {
                MessageBox.Show(e.Message, "ConnectionError", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private bool CanGetInfo(object CommandParameter) => true;

        private void OnListenPort(object commandParameter)
        {
            try
            {
                if (Int32.TryParse(ListenPort, out int prt) && (1023 < prt && prt < 49152))
                {
                    mainModel.SetName(Name);
                    mainModel.ListenVisible = false;
                    mainModel.InviteVisible = true;
                    mainModel.Listener(prt);
                    _getPortListenCommand.InvokeCanExecuteChanged();
                }
                else
                {
                    throw new InvalidInfoException("Invalid portnumber, must be a number between 1023 and 49152");
                }
            }
            catch (InvalidInfoException e)
            {
                MessageBox.Show(e.Message, "ListenError", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private bool CanListenPort(object CommandParameter) => true;

        private void OnSend(object commandParameter)
        {
            //Should make the string into a TextMessage or ImageMessage class object
            if (!string.IsNullOrEmpty(Message))
            {
                Thread t = new Thread(delegate ()
                {
                    try
                    {
                        mainModel.SendMessage(Message);
                        Message = "";
                    }
                    catch (InvalidInfoException e)
                    {
                        MessageBox.Show(e.Message, "SendError", MessageBoxButton.OK, MessageBoxImage.Error);
                    }
                });
                t.IsBackground = true;
                t.Start();
            }
            else
            {
                MessageBox.Show("That is not a valid message!", "ValidationError", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private bool CanSend(object CommandParameter) => true;

        private void OnAcceptInvite(object commandParameter)
        {
            mainModel.AcceptedRequest = true;
            mainModel.InviteVisible = false;
            mainModel.PendingInvite = false;
            mainModel.Messages.RemoveAll();
        }

        private bool CanAcceptInvite(object CommandParameter) => true;

        private void OnDeclineInvite(object commandParameter)
        {
            mainModel.AcceptedRequest = false;
            mainModel.PendingInvite = false;
            mainModel.Invite = "No pending invites";
        }

        private bool CanDeclineInvite(object CommandParameter) => true;

        private void OnDisconnect(object CommandParameter)
        {
            Thread thread = new Thread(delegate ()
            {
                mainModel.Disconnect();
            });
            thread.IsBackground = true;
            thread.Start();
        }

        private bool CanDisconnect(object CommandParameter) => true;

        private void OnShowConversation(object CommandParameter)
        {
            string dt = CommandParameter.ToString();
            mainModel.ShowConversation(dt);
        }

        private bool CanShowConversation(object CommandParameter) => true;

        private void OnSearchConversations(object CommandParameter)
        {
            string search = SearchParameter;

            Thread t = new Thread(delegate ()
            {
                Conversations = mainModel.FindConversations(search);
            });
            t.IsBackground = true;
            t.Start();
        }
        private bool CanSearchConversations(object CommandParameter) => true;

        private void OnBuzz(object obj)
        {
            Thread buzzThread = new Thread(delegate ()
            {
                try
                {
                    mainModel.SendBuzz();
                }
                catch (InvalidInfoException e)
                {
                    MessageBox.Show(e.Message, "Error!", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            });
            buzzThread.IsBackground = true;
            buzzThread.Start();
        }

        private bool CanBuzz(object arg) => true;

        private void OnAddImage(object obj)
        {
            OpenFileDialog op = new OpenFileDialog();
            op.Title = "Select a picture";
            op.Filter = "All supported graphics|*.jpg;*.jpeg;*.png|" +
                        "JPEG (*.jpg;*.jpeg)|*.jpg;*.jpeg|" +
                        "Portable Network Graphic (*.png)|*.png";
            //regex for checking type of encoder 
            if (op.ShowDialog() == true)
            {
                Thread t = new Thread(delegate ()
                {
                    try
                    {
                        Uri filePath = new Uri(op.FileName);
                        BitmapImage image = new BitmapImage(filePath);

                        mainModel.SendImage(image, filePath);
                    }
                    catch (InvalidInfoException e)
                    {
                        MessageBox.Show(e.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    }
                });
                t.IsBackground = true;
                t.Start();
            }
        }

        private bool CanAddImage(object arg) => true;
    }
}
