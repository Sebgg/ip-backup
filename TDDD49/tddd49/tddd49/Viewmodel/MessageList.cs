using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tddd49.Viewmodel
{
    class MessageList : ObservableCollection<Message>
    {
        public MessageList() 
            : base()
        {
        }
    }

    public partial class Message
    {
        public DateTime Date { get; set; }

        public string Sender { get; set; }

        public Message(string sender)
        {
            Date = DateTime.Now;
            Sender = sender;
        }

    }

    public class TextMessage : Message
    {
        public TextMessage(string sender, string message)
            : base(sender)
        {
            Message = message;
        }

        public string Message { get; set; }
    }
    public class ImageMessage : Message
    {
        public ImageMessage(string date, string sender /*PICTURE here as well, Uri?*/)
            : base(sender /*picturewhatever aswell*/)
        {

        }
    }
}
