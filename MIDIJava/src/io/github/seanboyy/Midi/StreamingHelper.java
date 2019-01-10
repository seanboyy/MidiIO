package io.github.seanboyy.Midi;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

class StreamingHelper {

    static byte[] streamValueToBitString(int[] cursor, byte[] bitString, int value) {
        for (int i = 0; i < 4; ++i) {
            bitString[cursor[0]++] = (byte) ((value << (8 * i)) >> (8 * 3));
        }
        return bitString;
    }

    static byte[] streamValueToBitString(int[] cursor, byte[] bitString, short value) {
        for (int i = 0; i < 2; ++i) {
            bitString[cursor[0]++] = (byte) ((value << (8 * i)) >> (8));
        }
        return bitString;
    }

    private static byte[] streamValueToBitString(int[] cursor, byte[] bitString, char value) {
        bitString[cursor[0]++] = (byte) value;
        return bitString;
    }

    static byte[] streamValueToBitString(int[] cursor, byte[] bitString, byte value) {
        return StreamingHelper.streamValueToBitString(cursor, bitString, (char) value);
    }

    static byte[] streamObjectToBitString(int[] cursor, byte[] bitString, VariableLengthValue object, long length) {
        byte[] objString = object.toBitString();
        for (long i = 0; i < length; ++i) {
            bitString[cursor[0]++] = objString[(int) i];
        }
        return bitString;
    }

    static byte[] streamObjectToBitString(int[] cursor, byte[] bitString, Event object, long length) {
        byte[] objString = object.toBitString();
        for (long i = 0; i < length; ++i) {
            bitString[cursor[0]++] = objString[(int) i];
        }
        return bitString;
    }

    static byte[] streamObjectToBitString(int[] cursor, byte[] bitString, Chunk object, long length) {
        byte[] objString = object.toBitString();
        for (long i = 0; i < length; ++i) {
            bitString[cursor[0]++] = objString[(int) i];
        }
        return bitString;
    }

    static byte getByteFromBitString(int[] cursor, byte[] bitString) {
        return bitString[cursor[0]++];
    }

    static short getShortFromBitString(int[] cursor, byte[] bitString) {
        ByteBuffer bb = ByteBuffer.allocate(2);
        bb.order(ByteOrder.BIG_ENDIAN);
        bb.put(bitString[cursor[0]++]);
        bb.put(bitString[cursor[0]++]);
        return bb.getShort(0);
    }

    static int getIntFromBitString(int[] cursor, byte[] bitString) {
        ByteBuffer bb = ByteBuffer.allocate(4);
        bb.order(ByteOrder.BIG_ENDIAN);
        bb.put(bitString[cursor[0]++]);
        bb.put(bitString[cursor[0]++]);
        bb.put(bitString[cursor[0]++]);
        bb.put(bitString[cursor[0]++]);
        return bb.getInt(0);
    }

    static VariableLengthValue getVariableLengthValueFromBitString(int[] cursor, byte[] bitString) {
        int length = 1;
        for (int i = 0; i < length; ++i) {
            byte tempByte = getByteFromBitString(cursor, bitString);
            if ((tempByte & 0x80) == 0x80) length++;
        }
        ByteBuffer bb = ByteBuffer.allocate(4);
        bb.order(ByteOrder.BIG_ENDIAN);
        cursor[0] -= length;
        for(int i = 0; i < 4 - length; ++i){
            bb.put((byte)0);
        }
        for (int i = 0; i < length; ++i) {
            bb.put(getByteFromBitString(cursor, bitString));
        }
        return new VariableLengthValue(VariableLengthValue.toNumber(bb.getInt(0)));
    }
}
