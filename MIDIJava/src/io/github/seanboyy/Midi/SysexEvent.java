package io.github.seanboyy.Midi;

import java.util.ArrayList;

public class SysexEvent implements Event{
    private byte eventType;
    private VariableLengthValue timeDelta;
    private ArrayList<Byte> eventData;
	private VariableLengthValue eventLength;

	public SysexEvent(VariableLengthValue timeDelta, byte eventType, VariableLengthValue eventLength, ArrayList<Byte> eventData){
		this.timeDelta = new VariableLengthValue(timeDelta);
		this.eventType = eventType;
		this.eventLength = new VariableLengthValue(eventLength);
		this.eventData = new ArrayList<>();
		this.eventData.addAll(eventData);
	}

	@Override
	public byte[] toBitString(){
		byte[] bitString = new byte[getLength()];
		int[] cursor = {0};
		bitString = StreamingHelper.streamObjectToBitString(cursor, bitString, timeDelta, timeDelta.getLength());
		bitString = StreamingHelper.streamValueToBitString(cursor, bitString, eventType);
        bitString = StreamingHelper.streamObjectToBitString(cursor, bitString, eventLength, eventLength.getLength());
        for(Byte b : eventData){
            bitString = StreamingHelper.streamValueToBitString(cursor, bitString, b);
        }
        return bitString;
    }

	@Override
	public int getLength(){
		return timeDelta.getLength() + 1 + eventLength.getLength() + eventData.size();
	}
}
