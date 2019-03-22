using System.Collections.Generic;

namespace MIDILibrary
{
    public class MidiEvent : IEvent
    {
        public VariableLengthValue TimeDelta { get; }
        public byte EventType { get; }
        public List<byte> EventData { get; }

        public MidiEvent(VariableLengthValue timeDelta, byte eventType, IEnumerable<byte> eventData)
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
            var bitString = new byte[GetLength()];
            uint cursor = 0;
            StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, TimeDelta, TimeDelta.Length);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, EventType);
            foreach (var b in EventData)
            {
                StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, b);
            }
            return bitString;
        }
    }
}
