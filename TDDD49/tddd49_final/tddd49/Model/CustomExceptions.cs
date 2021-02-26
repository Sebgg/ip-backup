using System;

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
