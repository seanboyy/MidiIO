package io.github.seanboyy.Midi;

import java.util.ArrayList;

public class MidiEvent implements Event {
    private VariableLengthValue timeDelta;
    private byte eventType;
    private ArrayList<Byte> eventData;

    MidiEvent(VariableLengthValue timeDelta, byte eventType, ArrayList<Byte> eventData) {
        this.timeDelta = new VariableLengthValue(timeDelta);
        this.eventType = eventType;
        this.eventData = new ArrayList<>();
        this.eventData.addAll(eventData);
    }

    @Override
    public byte[] toBitString() {
        byte[] bitString = new byte[getLength()];
        int[] cursor = {0};
        bitString = StreamingHelper.streamObjectToBitString(cursor, bitString, timeDelta, timeDelta.getLength());
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, eventType);
        for (Byte b : eventData) {
            bitString = StreamingHelper.streamValueToBitString(cursor, bitString, b);
        }
        return bitString;
    }

    @Override
    public int getLength() {
        return timeDelta.getLength() + 1 + eventData.size();
    }
}
