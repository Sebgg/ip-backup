using System;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Collections.Generic;
using tddd49.Model.ExtraClasses;

namespace tddd49.Viewmodel
{
    class MessageList : ObservableCollection<Message>
    {
        public MessageList() 
            : base()
        {
        }

        public MessageList(ConversationStorage list)
            : base()
        {
            foreach (Message m in list.Conversation)
            {
                Add(m);
            }
        }

        public void AddMessage(Message message)
        {
            Add(message);
        }

        public void RemoveAll()
        {
            Clear();
        }
    }

    public partial class Message
    {
        public string MessageContent { get; set; }

        public DateTime Date { get; set; }

        public string Sender { get; set; }

        public string MessageImage { get; set; }

        public Message(string sender, string content = "", Uri uri = null)
        {
            Date = DateTime.Now;
            Sender = sender;
            if (content != "")
            {
                MessageContent = content;
                MessageImage = "";
            }
            else if (uri != null)
            {
                MessageImage = uri.ToString();
            }
        }
    }

    public class TextMessage : Message
    {
        public TextMessage(string sender, string message)
            : base(sender, content: message)
        {
        }
    }
    public class ImageMessage : Message 
    {
        public ImageMessage(string sender, Uri imagelocation)
            : base(sender, uri: imagelocation)
        {
        }
    }
}
