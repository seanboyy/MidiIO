namespace MIDILibrary
{
    public class HeaderChunk : IChunk
    {
        private readonly uint _length;
        private readonly ushort _format;
        public ushort TrackCount { get; }
        public ushort TickDivision { get; }

        public HeaderChunk(uint length, ushort format, ushort trackCount, ushort tickDivision)
        {
            _length = length;
            _format = format;
            TrackCount = trackCount;
            TickDivision = tickDivision;
        }

        public uint GetLength()
        {
            return 8 + _length;
        }

        public byte[] ToBitString()
        {
            var bitString = new byte[GetLength()];
            uint cursor = 0;
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, MidiConstants.MIDI_SIGNATURE);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, _length);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, _format);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, TrackCount);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, TickDivision);
            return bitString;
        }
    }
}
