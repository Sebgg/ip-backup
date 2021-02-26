using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tddd49.Viewmodel
{
    class Conversation
    {
        public Conversation()
        {
        }

        public MessageList ConvMessages { get; set; }
        public string ConvName { get; set; }
        public DateTime ConvDate { get; set; }
    }
}
