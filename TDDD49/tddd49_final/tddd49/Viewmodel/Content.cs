using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tddd49.Viewmodel
{
    public class Content<T>
    {
        private T _value;

        public T Value
        {
            get => _value; set => _value = value;
        }

        public static implicit operator T(Content<T> value)
        {
            return value.Value;
        }

        public static implicit operator Content<T>(T value)
        {
            return new Content<T> { Value = value };
        }
    }
}
