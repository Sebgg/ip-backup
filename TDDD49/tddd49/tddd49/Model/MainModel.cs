using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using tddd49.Model;
using System.Threading;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Net.Sockets;
using Newtonsoft.Json;
using System.Windows.Threading;
using System.Windows;
using tddd49.Viewmodel;

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
        private bool _declinedRequest = false;
        private string _invite = "No pending invite";
        private string _recievedMessage = "";
        private bool _listening = false;
        private string _connectedTo = "";

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
            set => SetProperty(ref _notConnecting, value);
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
            set => SetProperty(ref _acceptedRequest, value);
        }
        public bool DeclinedRequest
        {
            get => _declinedRequest;
            set => SetProperty(ref _declinedRequest, value);
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

        public MainModel()
        {
            serverModel =  new ServerModel(this);
            clientModel = new ClientModel(this);

        }

        //Runs on a threadpool thread
        public void Connecter(string ipAddress, int port)
        {
            NotConnecting = false;
            IPAddress otherEnd = IPAddress.Parse(ipAddress); //ip address for computer to connect to
            try
            {
                clientModel.ConnectToPeer(otherEnd, port);
            }
            catch (Exception e)
            {
                // NullReferenceException: Object reference not set to an instance of an object.
                throw e;
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
                t2.IsBackground = true;
                t2.Start();
            }
            catch (Exception e)
            {
                throw new Exception("ListenConnectionError", e);
            }
        }

        public void SendMessage(string message)
        {
            try
            {
                Thread thread = new Thread(delegate ()
                {
                    if (Listening)
                    {
                        serverModel.Send(message);
                    }
                    else
                    {
                        clientModel.Send(message);
                    }
                });
                thread.IsBackground = true;
                thread.Start();
            }
            catch (Exception e)
            {
                throw new Exception("SendMessageError", e);
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        protected void SetProperty<T>(ref T field, T newValue, [CallerMemberName]string propertyName = null)
        {
            if (!EqualityComparer<T>.Default.Equals(field, newValue))
            {
                if (PropertyChanged != null)
                {
                    field = newValue;
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
                }
            }
        }
        
    }

    public class CustomProtocol
    {
        public string RequestType { get; set; } //establishconnection, declineconnection, acceptconnection, closeconnection, message
        public string SenderName { get; set; }
        public DateTime Date { get; set; }
        public string ReqIp { get; set; }
        public int ReqPort { get; set; }
    }
}
