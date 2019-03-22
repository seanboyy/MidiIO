using System.Collections.Generic;

namespace MIDILibrary
{
    public class TrackChunk : IChunk
    {
        private readonly uint _length;
        public List<IEvent> Events { get; }

        public TrackChunk(uint length, IEnumerable<IEvent> events)
        {
            _length = length;
            Events = new List<IEvent>();
            Events.AddRange(events);
        }
        
        public uint GetLength()
        {
            return 8 + _length;
        }

        public byte[] ToBitString()
        {
            byte[] bitString = new byte[GetLength()];
            uint cursor = 0;
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, MidiConstants.TRACK_SIGNATURE);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, _length);
            foreach (var e in Events)
            {
                StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, e, e.GetLength());
            }
            return bitString;
        }
    }
}
