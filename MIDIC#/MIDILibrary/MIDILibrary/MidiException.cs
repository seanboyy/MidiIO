using System;
using System.Runtime.Serialization;

namespace MIDILibrary
{
    [Serializable]
    internal class MidiException : Exception
    {
        public MidiException()
        {
        }

        public MidiException(string message) : base(message)
        {
            Console.Error.WriteLine(message);
        }

        public MidiException(string message, Exception innerException) : base(message, innerException)
        {
        }

        protected MidiException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
        }
    }
}