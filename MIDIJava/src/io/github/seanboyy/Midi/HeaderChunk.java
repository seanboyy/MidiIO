package io.github.seanboyy.Midi;

public class HeaderChunk implements Chunk {
    private int length;
    private short format;
    private short trackCount;
    private short tickDivision;

    public short getTickDivision() {
        return tickDivision;
    }

    short getTrackCount() {
        return trackCount;
    }

    HeaderChunk(int length, short format, short trackCount, short tickDivision) {
        this.length = length;
        this.format = format;
        this.trackCount = trackCount;
        this.tickDivision = tickDivision;
    }

    @Override
    public byte[] toBitString() {
        byte[] bitString = new byte[getLength()];
        int[] cursor = {0};
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, MidiConstants.MIDI_SIGNATURE);
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, length);
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, format);
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, trackCount);
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, tickDivision);
        return bitString;
    }

    @Override
    public int getLength() {
        return 8 + length;
    }
}
