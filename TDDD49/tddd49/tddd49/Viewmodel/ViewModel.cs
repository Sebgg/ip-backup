using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Text.RegularExpressions;
using System.Net;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using tddd49.Model;
using System.Threading;
using System.Windows;

namespace tddd49.Viewmodel
{
    internal class ViewModel : INotifyPropertyChanged
    {

        private string _ipAddress;
        private string _port;
        private string _connect = "Connect";
        //private bool _notConnecting = true;
        private string _listenport;
        private bool _listenVisible = true;
        private bool _inviteVisible = false;
        private bool _showDisconnect = false;
        private bool _showConnect = true;
        private string _message = "";

        // From Model
        private bool _connected = false;
        private string _name = "";
        private string _friendName = "";
        private bool _pendingInvite = false;
        private bool _acceptedRequest = false;
        private bool _declinedRequest = false;
        private string _invite = "No pending invites";
        private string _connectedTo;

        public MainModel mainModel;

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
            mainModel = model;
            mainModel.PropertyChanged += MainModel_PropertyChanged;
            
        }

        public string IpAddress
        {
            get => _ipAddress;
            set => SetProperty(ref _ipAddress, value);
        }
        public string Port
        {
            get => _port;
            set => SetProperty(ref _port, value);
        }
        public string Connect
        {
            get => _connect;
            set => SetProperty(ref _connect, value);
        }
        public string ListenPort
        {
            get => _listenport;
            set => SetProperty(ref _listenport, value);
        }
        public bool ListenVisible
        {
            get => _listenVisible;
            set => SetProperty(ref _listenVisible, value);
        }
        public bool InviteVisible
        {
            get => _inviteVisible;
            set => SetProperty(ref _inviteVisible, value);
        }
        public bool ShowDisconnect
        {
            get => _showDisconnect;
            set => SetProperty(ref _showDisconnect, value);
        }
        public bool ShowConnect
        {
            get => _showConnect;
            set => SetProperty(ref _showConnect, value);
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
                //mainModel.PendingInvite = value;
                _pendingInvite = value;
                OnPropertyChanged();
            }
        }
        public string Name
        {
            get => mainModel.Name;
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
            set => SetProperty(ref _acceptedRequest, value);
        }
        public bool DeclinedRequest
        {
            get => mainModel.DeclinedRequest;
            set => SetProperty(ref _declinedRequest, value);
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

 

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string propertyName = "")
        {
            Console.WriteLine("Property changed");
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        protected void SetProperty<T>(ref T field, T newValue, [CallerMemberName]string propertyName = null)
        {
            if (!EqualityComparer<T>.Default.Equals(field, newValue))
            {
                field = newValue;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        public void MainModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch(e.PropertyName)
            {
                case "Connected":
                    Connected = mainModel.Connected;
                    break;
                //case "NotConnecting":
                //    NotConnecting = mainModel.NotConnecting;
                //    break;
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
                case "DeclinedRequest":
                    DeclinedRequest = mainModel.DeclinedRequest;
                    break;
                case "Invite":
                    Invite = mainModel.Invite;
                    break;
                case "ConnectedTo":
                    ConnectedTo = mainModel.ConnectedTo;
                    break;
                default:
                    break;
            }
        }

        private void OnGetInfo(object commandParameter)
        {
            try
            {
                if (Connected == false)
                {
                    if (IPAddress.TryParse(IpAddress, out IPAddress _))
                    {
                        if (Int32.TryParse(Port, out int prt) && (1023 < prt && prt < 49152))
                        {
                            string ipAddress = IpAddress;
                            Thread thread = new Thread(delegate ()
                            {  
                                mainModel.Connecter(ipAddress, prt);
                            });
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
                else if (Connect == "Disconnect")
                {
                    Console.WriteLine("Here we go");
                    Connected = false;
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }

        private bool CanGetInfo(object CommandParameter)
        {
            return true;
        }

        private void OnListenPort(object commandParameter)
        {
            if(Int32.TryParse(ListenPort, out int prt) && (1023 < prt && prt < 49152))
            {
                try
                {
                    ListenVisible = false;
                    InviteVisible = true;
                    mainModel.Listener(prt);
                    _getPortListenCommand.InvokeCanExecuteChanged();
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message);
                }
            }
            else
            {
                throw new InvalidInfoException("Invalid portnumber, must be a number between 1023 and 49152");
            }
        }

        private bool CanListenPort(object CommandParameter)
        {
            return true;
        }

        private void OnSend(object commandParameter)
        {
            //Should make the string into a TextMessage or ImageMessage class object
            if (!String.IsNullOrEmpty(Message))
            {
                mainModel.SendMessage(Message);
            }
            else
            {
                MessageBox.Show(Application.Current.MainWindow, "That is not a valid message!", "Error");
            }
        }

        private bool CanSend(object CommandParameter)
        {
            return true;
        }

        private void OnAcceptInvite(object commandParameter)
        {
            mainModel.AcceptedRequest = true;
            InviteVisible = false;
            mainModel.PendingInvite = false;
        }

        private bool CanAcceptInvite(object CommandParameter)
        {
            return true;
        }
        private void OnDeclineInvite(object commandParameter)
        {
            mainModel.AcceptedRequest = false;
            mainModel.PendingInvite = false;
        }

        private bool CanDeclineInvite(object CommandParameter)
        {
            return true;
        }

        private void OnDisconnect(object CommandParameter)
        {
            mainModel.Connected = false;
            if (mainModel.Listening)
            {
                InviteVisible = true;
                mainModel.Invite = "No pending invites";
            }
            else
            {
                ListenVisible = true;
            }
        }

        private bool CanDisconnect(object CommandParameter)
        {
            return true;
        }
    }
}
