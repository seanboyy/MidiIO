using System.Collections.Generic;

namespace MIDILibrary
{
    public class MidiEvent : IEvent
    {
        public VariableLengthValue TimeDelta { get; }
        public byte EventType { get; }
        public List<byte> EventData { get; }

        public MidiEvent(VariableLengthValue timeDelta, byte eventType, List<byte> eventData)
        {
            TimeDelta = new VariableLengthValue(timeDelta);
            EventType = eventType;
            EventData = new List<byte>();
            EventData.AddRange(eventData);
        }

        public uint GetLength()
        {
            return TimeDelta.Length + 1 + (uint)EventData.Count;
        }

        public byte[] ToBitString()
        {
            byte[] bitString = new byte[GetLength()];
            uint cursor = 0;
            StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, TimeDelta, TimeDelta.Length);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, EventType);
            foreach (byte b in EventData)
            {
                StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, b);
            }
            return bitString;
        }
    }
}
