using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace tddd49
{
    class InvalidInfoException: ApplicationException
    {
        public InvalidInfoException()
        {
        }
        public InvalidInfoException(string message)
            : base(message)
        {
        }
        public InvalidInfoException(string message, Exception inner)
            : base(message, inner)
        {
        }

    }
}
