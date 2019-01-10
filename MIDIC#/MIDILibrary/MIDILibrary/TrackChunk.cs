using System.Collections.Generic;

namespace MIDILibrary
{
    public class TrackChunk : IChunk
    {
        private readonly uint length;
        public List<IEvent> Events { get; }

        public TrackChunk(uint length, List<IEvent> events)
        {
            this.length = length;
            Events = new List<IEvent>();
            Events.AddRange(events);
        }
        
        public uint GetLength()
        {
            return 8 + length;
        }

        public byte[] ToBitString()
        {
            byte[] bitString = new byte[GetLength()];
            uint cursor = 0;
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, MidiConstants.TRACK_SIGNATURE);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, length);
            foreach (IEvent e in Events)
            {
                StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, e, e.GetLength());
            }
            return bitString;
        }
    }
}
