package io.github.seanboyy.Midi;

import java.util.ArrayList;

public class Midi {

    private ArrayList<Chunk> chunks;

    public Midi() {
        this.chunks = new ArrayList<>();
    }

    public Midi(ArrayList<Chunk> chunks) {
        this.chunks = new ArrayList<>();
        this.chunks.addAll(chunks);
    }

    @Override
    public String toString(){
        return Formatter.formatMidi(this);
    }

    public byte[] toBitString() {
        byte[] bitString = new byte[getLength()];
        int[] cursor = {0};
        for (Chunk chunk : chunks) {
            bitString = StreamingHelper.streamObjectToBitString(cursor, bitString, chunk, chunk.getLength());
        }
        return bitString;
    }

    void fromBitString(byte[] bitString, int fileSize) {
        int[] cursor = {0};
        chunks = new ArrayList<>();
        int signature = StreamingHelper.getIntFromBitString(cursor, bitString);
        assert (signature == MidiConstants.MIDI_SIGNATURE);
        int length = StreamingHelper.getIntFromBitString(cursor, bitString);
        short format = StreamingHelper.getShortFromBitString(cursor, bitString);
        short tracks = StreamingHelper.getShortFromBitString(cursor, bitString);
        short division = StreamingHelper.getShortFromBitString(cursor, bitString);
        chunks.add(new HeaderChunk(length, format, tracks, division));
        while (cursor[0] < fileSize) {
            signature = StreamingHelper.getIntFromBitString(cursor, bitString);
            assert (signature == MidiConstants.TRACK_SIGNATURE);
            length = StreamingHelper.getIntFromBitString(cursor, bitString);
            int endOfTrack = length + cursor[0];
            ArrayList<Event> events = new ArrayList<>();
            while (cursor[0] < endOfTrack) {
                VariableLengthValue deltaTime = StreamingHelper.getVariableLengthValueFromBitString(cursor, bitString);
                byte eventType = StreamingHelper.getByteFromBitString(cursor, bitString);
                if (eventType == MidiConstants.META_EVENT) {
                    byte metaEventType = StreamingHelper.getByteFromBitString(cursor, bitString);
                    if (metaEventType == MidiConstants.END_OF_TRACK)
                        assert (cursor[0] + 1 == endOfTrack);
                    VariableLengthValue eventLength = StreamingHelper.getVariableLengthValueFromBitString(cursor, bitString);
                    ArrayList<Byte> eventData = new ArrayList<>();
                    if (eventLength.toNumber() != 0) {
                        for (int i = 0; i < eventLength.toNumber(); ++i) {
                            eventData.add(StreamingHelper.getByteFromBitString(cursor, bitString));
                        }
                    }
                    events.add(new MetaEvent(deltaTime, metaEventType, eventLength, eventData));
                } else if (Integer.compareUnsigned(eventType, MidiConstants.META_EVENT) < 0 && Integer.compareUnsigned(eventType, MidiConstants.SYSEX_EVENT) >= 0) {
                    VariableLengthValue eventLength = StreamingHelper.getVariableLengthValueFromBitString(cursor, bitString);
                    ArrayList<Byte> eventData = new ArrayList<>();
                    if (eventLength.toNumber() != 0) {
                        for (int i = 0; i < eventLength.toNumber(); ++i) {
                            eventData.add(StreamingHelper.getByteFromBitString(cursor, bitString));
                        }
                    }
                    events.add(new SysexEvent(deltaTime, eventType, eventLength, eventData));
                } else if (Integer.compareUnsigned(eventType, MidiConstants.SYSEX_EVENT) < 0 && Integer.compareUnsigned(eventType, MidiConstants.NOTE_OFF) >= 0) {
                    ArrayList<Byte> eventData = new ArrayList<>();
                    if (Integer.compareUnsigned(eventType, MidiConstants.PROGRAM) < 0 || Integer.compareUnsigned(eventType, MidiConstants.PITCH) >= 0) {
                        eventData.add(StreamingHelper.getByteFromBitString(cursor, bitString));
                        eventData.add(StreamingHelper.getByteFromBitString(cursor, bitString));
                    } else {
                        eventData.add(StreamingHelper.getByteFromBitString(cursor, bitString));
                    }
                    events.add(new MidiEvent(deltaTime, eventType, eventData));
                }
            }
            chunks.add(new TrackChunk(length, events));
        }
    }

    public int getLength() {
        int length = 0;
        for (Chunk chunk : chunks) {
            length += chunk.getLength();
        }
        return length;
    }

    ArrayList<Chunk> getChunks(){
        return chunks;
    }
}
