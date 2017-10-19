#include "Midi.h"

namespace mid {

	template<typename T>
	void StreamingHelper::streamValueToBitString(uint& cursor, char*& bitString, T value) {
		for (uint i = 0; i < sizeof(value); i++) {
			bitString[cursor] = (value << (8 * i)) >> (8 * (sizeof(value) - 1));
			cursor++;
		}
	}

	template<typename T>
	void StreamingHelper::streamObjectToBitString(uint& cursor, char*& bitString, T object, uint length) {
		char* objStr = object.toBitString();
		for (uint i = 0; i < length; i++) {
			bitString[cursor] = objStr[i];
			cursor++;
		}
	}
}