using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using tddd49.Viewmodel;
using Newtonsoft.Json;
using System.Windows;
using tddd49.Model.ExtraClasses;

namespace tddd49.Model
{
    class JsonStorage
    {
        private readonly string workingPath;
        private readonly DateTime time;
        private readonly string name;

        public JsonStorage(string friendName, bool isServer)
        {
            string path = @"History";
            string fullPath = Path.GetFullPath(path);

            string fileName = "conversations.json";

            if (isServer) // only for testing
            {
                fileName = "server" + fileName;
            }
            else
            {
                fileName = "client" + fileName;
            }
            workingPath = Path.Combine(fullPath, fileName);

            if (!File.Exists(workingPath))
            {
                File.Create(workingPath);
            }
            time = DateTime.Now;
            name = friendName;
        }

        public void addMessage(Message message)
        {
            try
            {
                var jsonData = File.ReadAllText(workingPath);

                var conversationList = JsonConvert.DeserializeObject<List<ConversationStorage>>(jsonData) ?? new List<ConversationStorage>();

                ConversationStorage conversation = null;

                foreach (ConversationStorage cc in conversationList)
                {
                    if (DateTime.Compare(cc.CreationTime, time) == 0)
                    {
                        conversation = cc;
                        conversationList.Remove(cc);
                        break;
                    }
                }
                if (conversation == null)
                {
                    conversation = new ConversationStorage(name, time);
                }

                conversation.Conversation.Add(message);

                conversationList.Add(conversation);

                var outJsonData = JsonConvert.SerializeObject(conversationList, Formatting.Indented);

                File.WriteAllText(workingPath, outJsonData);
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                Debug.WriteLine(e.StackTrace);
            }
        }
    }
}
