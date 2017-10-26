#include "Midi.h"

namespace mid {
	VariableLengthValue::VariableLengthValue(uint data) {
		toVariableLength(data);
	}

	VariableLengthValue::VariableLengthValue(const VariableLengthValue& other) {
		*this = other;
	}

	uint VariableLengthValue::toNumber() {
		uint result = 0;
		if (value <= VariableLengthValue::VLV_MAX_VAL) {
			uchar b1 = 0, b2 = 0, b3 = 0, b4 = 0;
			b4 |= (value & 0x0000007F);
			b4 |= (value & 0x00000100) >> 1;
			b3 |= (value & 0x00007E00) >> 9;
			b3 |= (value & 0x00030000) >> 10;
			b2 |= (value & 0x007C0000) >> 18;
			b2 |= (value & 0x07000000) >> 19;
			b1 |= (value & 0x78000000) >> 27;
			result |= b1;
			result <<= 8;
			result |= b2;
			result <<= 8;
			result |= b3;
			result <<= 8;
			result |= b4;
		}
		return result;
	}

	uint VariableLengthValue::toNumber(uint in) {
		uint result = 0;
		if (in <= VariableLengthValue::VLV_MAX_VAL) {
			uchar b1 = 0, b2 = 0, b3 = 0, b4 = 0;
			b4 |= (in & 0x0000007F);
			b4 |= (in & 0x00000100) >> 1;
			b3 |= (in & 0x00007E00) >> 9;
			b3 |= (in & 0x00030000) >> 10;
			b2 |= (in & 0x007C0000) >> 18;
			b2 |= (in & 0x07000000) >> 19;
			b1 |= (in & 0x78000000) >> 27;
			result = (result | b1) << 8;
			result = (result | b2) << 8;
			result = (result | b3) << 8;
			result = (result | b4) << 8;
		}
		return result;
	}

	void VariableLengthValue::toVariableLength(uint in) {
		value = 0;
		length = 0;
		assert(in <= VariableLengthValue::VLV_MAX_IN);
		if (in < 0x00000080) {
			length = 1;
			value = in;
		}
		else if (in < 0x00004000) {
			uchar b1, b2;
			b1 = 0x00000080;
			b2 = 0x00000000;
			b2 |= (in & 0x0000007F);
			b1 |= (in & 0x00003F80) >> 7;
			length = 2;
			value = (value | b1) << 8;
			value = (value | b2);
		}
		else if (in < 0x00200000) {
			uchar b1, b2, b3;
			b1 = 0x00000080;
			b2 = 0x00000080;
			b3 = 0x00000000;
			b3 |= (in & 0x0000007F);
			b2 |= (in & 0x00003F80) >> 7;
			b1 |= (in & 0x001FC000) >> 14;
			length = 3;
			value = (value | b1) << 8;
			value = (value | b2) << 8;
			value = (value | b3);

		}
		else if (in < 0x10000000) {
			uchar b1, b2, b3, b4;
			b1 = 0x00000080;
			b2 = 0x00000080;
			b3 = 0x00000080;
			b4 = 0x00000000;
			b4 |= (in & 0x0000007F);
			b3 |= (in & 0x00003F80) >> 7;
			b2 |= (in & 0x001FC000) >> 14;
			b1 |= (in & 0x0FE00000) >> 21;
			length = 4;
			value = (value | b1) << 8;
			value = (value | b2) << 8;
			value = (value | b3) << 8;
			value = (value | b4);
		}
	}

	char* VariableLengthValue::toBitString() {
		char* ret = new char[length];
		if (length == 1) {
			ret[0] = value;
		}
		else if (length == 2) {
			ret[0] = (value << 16) >> 24;
			ret[1] = (value << 24) >> 24;
		}
		else if (length == 3) {
			ret[0] = (value << 8) >> 24;
			ret[1] = (value << 16) >> 24;
			ret[2] = (value << 24) >> 24;
		}
		else if (length == 4) {
			ret[0] = value >> 24;
			ret[1] = (value << 8) >> 24;
			ret[2] = (value << 16) >> 24;
			ret[3] = (value << 24) >> 24;
		}
		return ret;
	}

	uint VariableLengthValue::getLength() {
		return length;
	}
	
	//---------------------BEGIN OPERATOR SECTION---------------------//

	VariableLengthValue::operator uint() {
		return toNumber();
	}

	void VariableLengthValue::operator= (uint rhs) {
		toVariableLength(rhs);
	}

	void VariableLengthValue::operator= (int rhs) {
		toVariableLength((uint)rhs);
	}

	void VariableLengthValue::operator= (const VariableLengthValue& rhs) {
		length = rhs.length;
		value = rhs.value;
	}

	void operator! (VariableLengthValue& lhs) {
		uint temp = lhs.toNumber() ^ 0xFFFFFFFF;
		if (temp > VariableLengthValue::VLV_MAX_IN) temp = VariableLengthValue::VLV_MAX_IN;
		lhs.toVariableLength(temp);
	}

	bool operator== (VariableLengthValue& lhs, uint rhs) {
		return (uint)lhs == rhs;
	}

	bool operator== (VariableLengthValue& lhs, int rhs) {
		return (uint)lhs == (uint)rhs;
	}

	bool operator!= (VariableLengthValue& lhs, uint rhs) {
		return !(lhs == rhs);
	}

	bool operator!= (VariableLengthValue& lhs, int rhs) {
		return !(lhs == rhs);
	}

	bool operator< (VariableLengthValue& lhs, uint rhs) {
		return (uint)lhs < rhs;
	}

	bool operator< (VariableLengthValue& lhs, int rhs) {
		return (uint)lhs < (uint)rhs;
	}

	bool operator> (VariableLengthValue& lhs, uint rhs) {
		return (uint)lhs > rhs;
	}

	bool operator> (VariableLengthValue& lhs, int rhs) {
		return (uint)lhs > (uint)rhs;
	}

	bool operator<= (VariableLengthValue& lhs, uint rhs) {
		return !(lhs > rhs);
	}

	bool operator<= (VariableLengthValue& lhs, int rhs) {
		return !(lhs > rhs);
	}

	bool operator>= (VariableLengthValue& lhs, uint rhs) {
		return !(lhs < rhs);
	}

	bool operator>= (VariableLengthValue& lhs, int rhs) {
		return !(lhs < rhs);
	}

	VariableLengthValue operator+ (VariableLengthValue& lhs, uint rhs) {
		return ((uint)lhs + rhs) > VariableLengthValue::VLV_MAX_IN ? VariableLengthValue(VariableLengthValue::VLV_MAX_IN) : VariableLengthValue((uint)lhs + rhs);
	}

	VariableLengthValue operator+ (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs + (uint)rhs);
	}

	VariableLengthValue operator- (VariableLengthValue& lhs, uint rhs) {
		return (int)((uint)lhs - rhs) < 0 ? VariableLengthValue(0) : VariableLengthValue((uint)lhs - rhs);
	}

	VariableLengthValue operator- (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs - (uint)rhs);
	}

	VariableLengthValue operator* (VariableLengthValue& lhs, uint rhs) {
		return ((uint)lhs * rhs) > VariableLengthValue::VLV_MAX_IN ? VariableLengthValue(VariableLengthValue::VLV_MAX_IN) : VariableLengthValue((uint)lhs * rhs);
	}

	VariableLengthValue operator* (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs * (uint)rhs);
	}

	VariableLengthValue operator/ (VariableLengthValue& lhs, uint rhs) {
		return ((uint)lhs / rhs) > VariableLengthValue::VLV_MAX_IN ? VariableLengthValue(VariableLengthValue::VLV_MAX_IN) : VariableLengthValue((uint)lhs / rhs);
	}

	VariableLengthValue operator/ (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs / (uint)rhs);
	}

	VariableLengthValue operator<< (VariableLengthValue& lhs, uint rhs) {
		return ((uint)lhs << rhs) > VariableLengthValue::VLV_MAX_IN ? VariableLengthValue(VariableLengthValue::VLV_MAX_IN) : VariableLengthValue((uint)lhs << rhs);
	}

	VariableLengthValue operator<< (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs << (uint)rhs);
	}

	VariableLengthValue& operator+= (VariableLengthValue& lhs, uint rhs) {
		lhs = lhs + rhs;
		return lhs;
	}

	VariableLengthValue& operator+= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs + rhs;
		return lhs;
	}

	VariableLengthValue& operator-= (VariableLengthValue& lhs, uint rhs) {
		lhs = lhs - rhs;
		return lhs;
	}

	VariableLengthValue& operator-= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs - rhs;
		return lhs;
	}

	VariableLengthValue& operator*= (VariableLengthValue& lhs, uint rhs) {
		lhs = lhs * rhs;
		return lhs;
	}

	VariableLengthValue& operator*= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs * rhs;
		return lhs;
	}

	VariableLengthValue& operator/= (VariableLengthValue& lhs, uint rhs) {
		lhs = lhs / rhs;
		return lhs;
	}

	VariableLengthValue& operator/= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs / rhs;
		return lhs;
	}

	VariableLengthValue& operator<<= (VariableLengthValue& lhs, uint rhs) {
		lhs = lhs << rhs;
		return lhs;
	}

	VariableLengthValue& operator<<= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs << rhs;
		return lhs;
	}

	VariableLengthValue& operator++ (VariableLengthValue& operand) {
		operand.toVariableLength((uint)(operand + 1U));
		return operand;
	}

	VariableLengthValue operator++ (VariableLengthValue& operand, int amt) {
		VariableLengthValue temp = operand;
		if (amt != 0) operand.toVariableLength((uint)(operand + (uint)amt));
		else operand.toVariableLength((uint)(operand + 1U));
		return temp;
	}

	VariableLengthValue& operator-- (VariableLengthValue& operand) {
		operand.toVariableLength((uint)(operand + 1U));
		return operand;
	}

	VariableLengthValue operator-- (VariableLengthValue& operand, int amt) {
		VariableLengthValue temp = operand;
		if (amt != 0) operand.toVariableLength((uint)(operand + (uint)amt));
		else operand.toVariableLength((uint)(operand + 1U));
		return temp;
	}
}