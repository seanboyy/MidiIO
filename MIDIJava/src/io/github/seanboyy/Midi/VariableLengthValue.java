package io.github.seanboyy.Midi;

class VariableLengthValue {
	private int length;
	private int value;

	private static final int VLV_MAX_IN = 0x0FFFFFFF;
	private static final int VLV_MAX_VAL = 0xFFFFFF7F;

	VariableLengthValue(int data) {
		toVariableLength(data);
	}

	VariableLengthValue(VariableLengthValue other) {
		length = other.length;
		value = other.value;
	}

	int toNumber() {
		int result = 0;
		if (Integer.compareUnsigned(value, VariableLengthValue.VLV_MAX_VAL) <= 0) {
			byte b1 = 0, b2 = 0, b3 = 0, b4 = 0;
			b4 |= (value & 0x0000007F);
			b4 |= ((value & 0x00000100) >>> 1);
			b3 |= ((value & 0x00007E00) >>> 9);
			b3 |= ((value & 0x00030000) >>> 10);
			b2 |= ((value & 0x007C0000) >>> 18);
			b2 |= ((value & 0x07000000) >>> 19);
			b1 |= ((value & 0x78000000) >>> 27);
			result |= (b1 & 0xFF);
			result <<= 8;
			result |= (b2 & 0xFF);
			result <<= 8;
			result |= (b3 & 0xFF);
			result <<= 8;
			result |= (b4 & 0xFF);
		}
		return result;
	}

	static int toNumber(int in) {
		int result = 0;
		if (Integer.compareUnsigned(in, VariableLengthValue.VLV_MAX_VAL) <= 0) {
			byte b1 = 0, b2 = 0, b3 = 0, b4 = 0;
			b4 |= (in & 0x0000007F);
			b4 |= ((in & 0x00000100) >>> 1);
			b3 |= ((in & 0x00007E00) >>> 9);
			b3 |= ((in & 0x00030000) >>> 10);
			b2 |= ((in & 0x007C0000) >>> 18);
			b2 |= ((in & 0x07000000) >>> 19);
			b1 |= ((in & 0x78000000) >>> 27);
			result |= (b1 & 0xFF);
			result <<= 8;
			result |= (b2 & 0xFF);
			result <<= 8;
			result |= (b3 & 0xFF);
			result <<= 8;
			result |= (b4 & 0xFF);
		}
		return result;
	}

	private void toVariableLength(int in) {
		value = 0;
		length = 0;
		//System.out.printf("%d, %d\n", in, VariableLengthValue.VLV_MAX_IN);
		assert (Integer.compareUnsigned(in, VariableLengthValue.VLV_MAX_IN) <= 0);
		if (Integer.compareUnsigned(in, 0x00000080) < 0) {
			length = 1;
			value = in;
		} else if (Integer.compareUnsigned(in, 0x00004000) < 0) {
			byte b1, b2;
			b1 = (byte) 0x80;
			b2 = 0;
			b2 |= (in & 0x0000007F);
			b1 |= ((in & 0x00003F80) >>> 7);
			length = 2;
			value |= (b1 & 0xFF);
			value <<= 8;
			value |= (b2 & 0xFF);
		} else if (Integer.compareUnsigned(in, 0x00200000) < 0) {
			byte b1, b2, b3;
			b1 = (byte) 0x80;
			b2 = (byte) 0x80;
			b3 = 0x00;
			b3 |= (in & 0x0000007F);
			b2 |= ((in & 0x00003F80) >>> 7);
			b1 |= ((in & 0x001FC000) >>> 14);
			length = 3;
			value |= (b1 & 0xFF);
			value <<= 8;
			value |= (b2 & 0xFF);
			value <<= 8;
			value |= (b3 & 0xFF);
		} else if (Integer.compareUnsigned(in, 0x10000000) < 0) {
			byte b1, b2, b3, b4;
			b1 = (byte) 0x80;
			b2 = (byte) 0x80;
			b3 = (byte) 0x80;
			b4 = 0x00;
			b4 |= (in & 0x0000007F);
			b3 |= ((in & 0x00003F80) >>> 7);
			b2 |= ((in & 0x001FC000) >>> 14);
			b1 |= ((in & 0x0FE00000) >>> 21);
			length = 4;
			value |= (b1 & 0xFF);
			value <<= 8;
			value |= (b2 & 0xFF);
			value <<= 8;
			value |= (b3 & 0xFF);
			value <<= 8;
			value |= (b4 & 0xFF);
		}
	}

	byte[] toBitString() {
		byte[] bitString = new byte[length];
		switch (length) {
			case 1:
				bitString[0] = (byte) value;
				break;
			case 2:
				bitString[0] = (byte) ((value << 16) >>> 24);
				bitString[1] = (byte) ((value << 24) >>> 24);
				break;
			case 3:
				bitString[0] = (byte) ((value << 8) >>> 24);
				bitString[1] = (byte) ((value << 16) >>> 24);
				bitString[2] = (byte) ((value << 24) >>> 24);
				break;
			case 4:
				bitString[0] = (byte) (value >>> 24);
				bitString[1] = (byte) ((value << 8) >>> 24);
				bitString[2] = (byte) ((value << 16) >>> 24);
				bitString[3] = (byte) ((value << 24) >>> 24);
		}
		return bitString;
	}

	int getLength() {
		return length;
	}

	int getValue() {
		return value;
	}
}
