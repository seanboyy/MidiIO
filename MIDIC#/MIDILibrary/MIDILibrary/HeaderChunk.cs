namespace MIDILibrary
{
    public class HeaderChunk : IChunk
    {
        private readonly uint length;
        private readonly ushort format;
        public ushort TrackCount { get; }
        public ushort TickDivision { get; }

        public HeaderChunk(uint length, ushort format, ushort trackCount, ushort tickDivision)
        {
            this.length = length;
            this.format = format;
            TrackCount = trackCount;
            TickDivision = tickDivision;
        }

        public uint GetLength()
        {
            return 8 + length;
        }

        public byte[] ToBitString()
        {
            byte[] bitString = new byte[GetLength()];
            uint cursor = 0;
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, MidiConstants.MIDI_SIGNATURE);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, length);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, format);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, TrackCount);
            StreamingHelper.StreamValueToBitString(ref cursor, ref bitString, TickDivision);
            return bitString;
        }
    }
}
