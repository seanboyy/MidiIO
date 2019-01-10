using System.Collections.Generic;

namespace MIDILibrary
{
    public class SysexEvent : IEvent
    {
        public byte EventType { get; }
        public VariableLengthValue TimeDelta { get; }
        public List<byte> EventData { get; }
        public VariableLengthValue EventLength { get; }

        public SysexEvent(VariableLengthValue timeDelta, byte eventType, VariableLengthValue eventLength, List<byte> eventData)
        {
            TimeDelta = new VariableLengthValue(timeDelta);
            EventType = eventType;
            EventLength = new VariableLengthValue(eventLength);
            EventData = new List<byte>();
            EventData.AddRange(eventData);
        }

        public byte[] ToBitString()
        {
            byte[] bitString = new byte[GetLength()];
            uint cursor = 0;
            StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, TimeDelta, TimeDelta.Length);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, EventType);
            StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, EventLength, EventLength.Length);
            foreach (byte b in EventData)
            {
                StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, b);
            }
            return bitString;
        }

        public uint GetLength()
        {
            return TimeDelta.Length + 1 + EventLength.Length + (uint)EventData.Count;
        }
    }
}
