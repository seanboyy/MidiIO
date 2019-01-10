package io.github.seanboyy.Midi;

import java.util.ArrayList;

public class MetaEvent implements Event {

    private ArrayList<Byte> eventData;
    private VariableLengthValue timeDelta;
    private VariableLengthValue eventLength;

    public ArrayList<Byte> getEventData() {
        return eventData;
    }

    public VariableLengthValue getTimeDelta() {
        return timeDelta;
    }

    public VariableLengthValue getEventLength() {
        return eventLength;
    }

    public byte getMetaEventType() {
        return metaEventType;
    }

    private byte metaEventType;

    MetaEvent(VariableLengthValue timeDelta, byte metaEventType, VariableLengthValue eventLength, ArrayList<Byte> eventData) {
        this.eventData = new ArrayList<>();
        this.eventData.addAll(eventData);
        this.timeDelta = new VariableLengthValue(timeDelta);
        this.metaEventType = metaEventType;
        this.eventLength = new VariableLengthValue(eventLength);
    }

    public byte[] toBitString() {
        byte[] bitString = new byte[getLength()];
        int[] cursor = {0};
        bitString = StreamingHelper.streamObjectToBitString(cursor, bitString, timeDelta, timeDelta.getLength());
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, MidiConstants.META_EVENT);
        bitString = StreamingHelper.streamValueToBitString(cursor, bitString, metaEventType);
        bitString = StreamingHelper.streamObjectToBitString(cursor, bitString, eventLength, eventLength.getLength());
        for (Byte b : eventData) {
            bitString = StreamingHelper.streamValueToBitString(cursor, bitString, b);
        }
        return bitString;
    }

    public int getLength() {
        return timeDelta.getLength() + 2 + eventLength.getLength() + eventData.size();
    }
}