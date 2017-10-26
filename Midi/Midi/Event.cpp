#include "Midi.h"

namespace mid {
	MetaEvent::MetaEvent(VariableLengthValue timeDelta, uchar metaEventType, VariableLengthValue eventLength, std::vector<uchar> eventData) {
		this->timeDelta = timeDelta;
		this->eventType = 0xFF;
		this->eventData = eventData;
		this->metaEventType = metaEventType;
		this->eventLength = eventLength;
	}

	char* MetaEvent::toBitString() {
		char* ret = new char[getLength()];
		uint cursor = 0;
		sh::streamObjectToBitString(cursor, ret, timeDelta, timeDelta.getLength());
		sh::streamValueToBitString(cursor, ret, eventType);
		sh::streamValueToBitString(cursor, ret, metaEventType);
		sh::streamObjectToBitString(cursor, ret, eventLength, eventLength.getLength());
		for (uchar byte : eventData) {
			sh::streamValueToBitString(cursor, ret, byte);
		}
		return ret;
	}

	void MetaEvent::fromBitString(uint& cursor, char* bitString) {
		timeDelta = sh::getVLVFromBitString(cursor, bitString);

	}

	uint MetaEvent::getLength() {
		return timeDelta.getLength() + sizeof(eventType) + sizeof(metaEventType) + eventLength.getLength() + eventData.size();
	}

	//---------------------END OF METAEVENT CLASS---------------------//

	SysexEvent::SysexEvent(VariableLengthValue timeDelta, uchar eventType, VariableLengthValue eventLength, std::vector<uchar> eventData) {
		this->timeDelta = timeDelta;
		this->eventType = eventType;
		assert(eventType == 0xF0 || eventType == 0xF7);
		this->eventData = eventData;
		this->eventLength = eventLength;
	}

	char* SysexEvent::toBitString() {
		char* ret = new char[getLength()];
		uint cursor = 0;
		sh::streamObjectToBitString(cursor, ret, timeDelta, timeDelta.getLength());
		sh::streamValueToBitString(cursor, ret, eventType);
		sh::streamObjectToBitString(cursor, ret, eventLength, eventLength.getLength());
		for (uchar byte : eventData) {
			sh::streamValueToBitString(cursor, ret, byte);
		}
		return ret;
	}

	uint SysexEvent::getLength() {
		return timeDelta.getLength() + sizeof(eventType) + eventLength.getLength() + eventData.size();
	}

	//---------------------END OF SYSEXEVENT CLASS---------------------//

	MidiEvent::MidiEvent(VariableLengthValue timeDelta, uchar eventType, std::vector<uchar> eventData) {
		this->timeDelta = timeDelta;
		this->eventType = eventType;
		this->eventData = eventData;
	}

	char* MidiEvent::toBitString() {
		char* ret = new char[getLength()];
		uint cursor = 0;
		sh::streamObjectToBitString(cursor, ret, timeDelta, timeDelta.getLength());
		sh::streamValueToBitString(cursor, ret, eventType);
		for (uchar byte : eventData) {
			sh::streamValueToBitString(cursor, ret, byte);
		}
		return ret;
	}

	uint MidiEvent::getLength() {
		return timeDelta.getLength() + sizeof(eventType) + eventData.size();
	}
}