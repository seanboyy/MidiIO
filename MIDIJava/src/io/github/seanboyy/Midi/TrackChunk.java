package io.github.seanboyy.Midi;

import java.util.ArrayList;

public class TrackChunk implements Chunk {

    private int length;

    private ArrayList<Event> events;


    ArrayList<Event> getEvents() {
        return events;
    }

    TrackChunk(int length, ArrayList<Event> events) {
        this.length = length;
        this.events = new ArrayList<>();
        this.events.addAll(events);
    }

    @Override
    public byte[] toBitString() {
        byte[] bitString = new byte[getLength()];
        int[] cursor = {0};
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, MidiConstants.TRACK_SIGNATURE);
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, length);
        for (Event e : events) {
            bitString = StreamingHelper.streamObjectToBitString(cursor, bitString, e, e.getLength());
        }
        return bitString;
    }

    @Override
    public int getLength() {
        return 8 + length;
    }
}
