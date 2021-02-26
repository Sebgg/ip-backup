using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using tddd49.Viewmodel;

namespace tddd49.Model.ExtraClasses
{
    public class ConversationStorage
    {
        public string ChatWith { get; set; }
        public DateTime CreationTime { get; set; }
        public List<Message> Conversation { get; set; }

        public ConversationStorage(string friendName, DateTime time)
        {
            ChatWith = friendName;
            CreationTime = time;
            Conversation = new List<Message>();
        }
    }
}
