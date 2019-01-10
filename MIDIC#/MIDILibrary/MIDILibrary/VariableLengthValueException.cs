using System;
using System.Runtime.Serialization;

namespace MIDILibrary
{
    [Serializable]
    internal class VariableLengthValueException : Exception
    {
        public VariableLengthValueException()
        {
        }

        public VariableLengthValueException(string message) : base(message)
        {
            Console.Error.WriteLine(message);
        }

        public VariableLengthValueException(string message, Exception innerException) : base(message, innerException)
        {
        }

        protected VariableLengthValueException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
        }
    }
}