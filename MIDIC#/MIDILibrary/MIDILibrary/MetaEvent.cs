using System.Collections.Generic;

namespace MIDILibrary
{
    public class MetaEvent : IEvent
    {
        public List<byte> EventData { get; }
        public VariableLengthValue TimeDelta { get; }
        public VariableLengthValue EventLength { get; }
        public byte MetaEventType { get; }

        public MetaEvent(VariableLengthValue timeDelta, byte metaEventType, VariableLengthValue eventLength, IEnumerable<byte> eventData)
        {
            EventData = new List<byte>();
            EventData.AddRange(eventData);
            TimeDelta = new VariableLengthValue(timeDelta);
            MetaEventType = metaEventType;
            EventLength = new VariableLengthValue(eventLength);
        }

        public uint GetLength()
        {
            return TimeDelta.Length + 2 + EventLength.Length + (uint)EventData.Count;
        }

        public byte[] ToBitString()
        {
            var bitString = new byte[GetLength()];
            uint cursor = 0;
            StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, TimeDelta, TimeDelta.Length);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, MidiConstants.META_EVENT);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, MetaEventType);
            StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, EventLength, EventLength.Length);
            foreach(var b in EventData)
            {
                StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, b);
            }
            return bitString;
        }
    }
}
