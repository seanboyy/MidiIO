#include "Midi.h"

namespace mid {
	template<typename T>
	void StreamingHelper::streamValueToBitString(uint32_t& cursor, char*& bitString, T value) {
		for (uint32_t i = 0; i < sizeof(value); i++) {
			bitString[cursor] = (value << (8 * i)) >> (8 * (sizeof(value) - 1));
			cursor++;
		}
	}

	template<typename T>
	void StreamingHelper::streamObjectToBitString(uint32_t& cursor, char*& bitString, T object, uint32_t length) {
		char* objStr = object.toBitString();
		for (uint32_t i = 0; i < length; i++) {
			bitString[cursor] = objStr[i];
			cursor++;
		}
	}

	template<typename T>
	void StreamingHelper::streamObjectToBitString(uint32_t& cursor, char*& bitString, T* object, uint32_t length) {
		char* objStr = object->toBitString();
		for (uint32_t i = 0; i < length; i++) {
			bitString[cursor] = objStr[i];
			cursor++;
		}
	}

	uint8_t StreamingHelper::getCharFromBitString(uint32_t& cursor, const char* bitString) {
		return bitString[cursor++];
	}

	uint16_t StreamingHelper::getShortFromBitString(uint32_t& cursor, const char* bitString) {
		uint16_t result = (uint8_t)bitString[cursor++];
		result <<= 8;
		result |= (uint8_t)bitString[cursor++];
		return result;
	}

	uint32_t StreamingHelper::getIntFromBitString(uint32_t& cursor, const char* bitString) {
		uint32_t result = (uint8_t)bitString[cursor++];
		result <<= 8;
		result = (result | (uint8_t)bitString[cursor++]) << 8;
		result = (result | (uint8_t)bitString[cursor++]) << 8;
		result |= (uint8_t)bitString[cursor++];
		return result;
	}

	VariableLengthValue StreamingHelper::getVLVFromBitString(uint32_t& cursor, const char* bitString) {
		uint32_t length = 1;
		uint32_t value = 0;
		for (uint32_t i = 0; i < length; i++) {
			uint8_t tempByte = getCharFromBitString(cursor, bitString);
			if ((tempByte & 0x80) == 0x80) length++;
			value |= tempByte;
			if (!(i == length - 1)) value <<= 8;
		}
		return VariableLengthValue(VariableLengthValue::toNumber(value));
	}

	//---------------------END OF STREAMINGHELPER CLASS---------------------//


	VariableLengthValue::VariableLengthValue(uint32_t data) {
		toVariableLength(data);
	}

	VariableLengthValue::VariableLengthValue(const VariableLengthValue& other) {
		*this = other;
	}

	uint32_t VariableLengthValue::toNumber() {
		uint32_t result = 0;
		if (value <= VariableLengthValue::VLV_MAX_VAL) {
			uint8_t b1 = 0, b2 = 0, b3 = 0, b4 = 0;
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

	uint32_t VariableLengthValue::toNumber(uint32_t in) {
		uint32_t result = 0;
		if (in <= VariableLengthValue::VLV_MAX_VAL) {
			uint8_t b1 = 0, b2 = 0, b3 = 0, b4 = 0;
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
			result |= b4;
		}
		return result;
	}

	void VariableLengthValue::toVariableLength(uint32_t in) {
		value = 0;
		length = 0;
		assert(in <= VariableLengthValue::VLV_MAX_IN);
		if (in < 0x00000080) {
			length = 1;
			value = in;
		}
		else if (in < 0x00004000) {
			uint8_t b1, b2;
			b1 = 0x00000080;
			b2 = 0x00000000;
			b2 |= (in & 0x0000007F);
			b1 |= (in & 0x00003F80) >> 7;
			length = 2;
			value = (value | b1) << 8;
			value = (value | b2);
		}
		else if (in < 0x00200000) {
			uint8_t b1, b2, b3;
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
			uint8_t b1, b2, b3, b4;
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

	uint32_t VariableLengthValue::getLength() {
		return length;
	}

		//---------------------BEGIN OPERATOR SECTION---------------------//

	VariableLengthValue::operator uint32_t() {
		return toNumber();
	}

	void VariableLengthValue::operator= (uint32_t rhs) {
		toVariableLength(rhs);
	}

	void VariableLengthValue::operator= (int rhs) {
		toVariableLength((uint32_t)rhs);
	}

	void VariableLengthValue::operator= (const VariableLengthValue& rhs) {
		length = rhs.length;
		value = rhs.value;
	}

	void operator! (VariableLengthValue& lhs) {
		uint32_t temp = lhs.toNumber() ^ 0xFFFFFFFF;
		if (temp > VariableLengthValue::VLV_MAX_IN) temp = VariableLengthValue::VLV_MAX_IN;
		lhs.toVariableLength(temp);
	}

	bool operator== (VariableLengthValue& lhs, uint32_t rhs) {
		return (uint32_t)lhs == rhs;
	}

	bool operator== (VariableLengthValue& lhs, int rhs) {
		return (uint32_t)lhs == (uint32_t)rhs;
	}

	bool operator!= (VariableLengthValue& lhs, uint32_t rhs) {
		return !(lhs == rhs);
	}

	bool operator!= (VariableLengthValue& lhs, int rhs) {
		return !(lhs == rhs);
	}

	bool operator< (VariableLengthValue& lhs, uint32_t rhs) {
		return (uint32_t)lhs < rhs;
	}

	bool operator< (VariableLengthValue& lhs, int rhs) {
		return (uint32_t)lhs < (uint32_t)rhs;
	}

	bool operator> (VariableLengthValue& lhs, uint32_t rhs) {
		return (uint32_t)lhs > rhs;
	}

	bool operator> (VariableLengthValue& lhs, int rhs) {
		return (uint32_t)lhs > (uint32_t)rhs;
	}

	bool operator<= (VariableLengthValue& lhs, uint32_t rhs) {
		return !(lhs > rhs);
	}

	bool operator<= (VariableLengthValue& lhs, int rhs) {
		return !(lhs > rhs);
	}

	bool operator>= (VariableLengthValue& lhs, uint32_t rhs) {
		return !(lhs < rhs);
	}

	bool operator>= (VariableLengthValue& lhs, int rhs) {
		return !(lhs < rhs);
	}

	VariableLengthValue operator+ (VariableLengthValue& lhs, uint32_t rhs) {
		return ((uint32_t)lhs + rhs) > VariableLengthValue::VLV_MAX_IN ? VariableLengthValue(VariableLengthValue::VLV_MAX_IN) : VariableLengthValue((uint32_t)lhs + rhs);
	}

	VariableLengthValue operator+ (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs + (uint32_t)rhs);
	}

	VariableLengthValue operator- (VariableLengthValue& lhs, uint32_t rhs) {
		return (int)((uint32_t)lhs - rhs) < 0 ? VariableLengthValue(0) : VariableLengthValue((uint32_t)lhs - rhs);
	}

	VariableLengthValue operator- (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs - (uint32_t)rhs);
	}

	VariableLengthValue operator* (VariableLengthValue& lhs, uint32_t rhs) {
		return ((uint32_t)lhs * rhs) > VariableLengthValue::VLV_MAX_IN ? VariableLengthValue(VariableLengthValue::VLV_MAX_IN) : VariableLengthValue((uint32_t)lhs * rhs);
	}

	VariableLengthValue operator* (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs * (uint32_t)rhs);
	}

	VariableLengthValue operator/ (VariableLengthValue& lhs, uint32_t rhs) {
		return ((uint32_t)lhs / rhs) > VariableLengthValue::VLV_MAX_IN ? VariableLengthValue(VariableLengthValue::VLV_MAX_IN) : VariableLengthValue((uint32_t)lhs / rhs);
	}

	VariableLengthValue operator/ (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs / (uint32_t)rhs);
	}

	VariableLengthValue operator<< (VariableLengthValue& lhs, uint32_t rhs) {
		return ((uint32_t)lhs << rhs) > VariableLengthValue::VLV_MAX_IN ? VariableLengthValue(VariableLengthValue::VLV_MAX_IN) : VariableLengthValue((uint32_t)lhs << rhs);
	}

	VariableLengthValue operator<< (VariableLengthValue& lhs, int rhs) {
		return VariableLengthValue(lhs << (uint32_t)rhs);
	}

	VariableLengthValue& operator+= (VariableLengthValue& lhs, uint32_t rhs) {
		lhs = lhs + rhs;
		return lhs;
	}

	VariableLengthValue& operator+= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs + rhs;
		return lhs;
	}

	VariableLengthValue& operator-= (VariableLengthValue& lhs, uint32_t rhs) {
		lhs = lhs - rhs;
		return lhs;
	}

	VariableLengthValue& operator-= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs - rhs;
		return lhs;
	}

	VariableLengthValue& operator*= (VariableLengthValue& lhs, uint32_t rhs) {
		lhs = lhs * rhs;
		return lhs;
	}

	VariableLengthValue& operator*= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs * rhs;
		return lhs;
	}

	VariableLengthValue& operator/= (VariableLengthValue& lhs, uint32_t rhs) {
		lhs = lhs / rhs;
		return lhs;
	}

	VariableLengthValue& operator/= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs / rhs;
		return lhs;
	}

	VariableLengthValue& operator<<= (VariableLengthValue& lhs, uint32_t rhs) {
		lhs = lhs << rhs;
		return lhs;
	}

	VariableLengthValue& operator<<= (VariableLengthValue& lhs, int rhs) {
		lhs = lhs << rhs;
		return lhs;
	}

	VariableLengthValue& operator++ (VariableLengthValue& operand) {
		operand.toVariableLength((uint32_t)(operand + 1U));
		return operand;
	}

	VariableLengthValue operator++ (VariableLengthValue& operand, int amt) {
		VariableLengthValue temp = operand;
		if (amt != 0) operand.toVariableLength((uint32_t)(operand + (uint32_t)amt));
		else operand.toVariableLength((uint32_t)(operand + 1U));
		return temp;
	}

	VariableLengthValue& operator-- (VariableLengthValue& operand) {
		operand.toVariableLength((uint32_t)(operand + 1U));
		return operand;
	}

	VariableLengthValue operator-- (VariableLengthValue& operand, int amt) {
		VariableLengthValue temp = operand;
		if (amt != 0) operand.toVariableLength((uint32_t)(operand + (uint32_t)amt));
		else operand.toVariableLength((uint32_t)(operand + 1U));
		return temp;
	}

	//---------------------END OF VARIABLELENGTHVALUE CLASS---------------------//

	TrackChunk::TrackChunk(uint32_t length, std::vector<Event*> events) {
		this->chunkSignature = c::TRACK_SIGNATURE;
		this->length = length;
		this->events = events;
	}

	char* TrackChunk::toBitString() {
		char* ret = new char[getLength()];
		uint32_t cursor = 0;
		sh::streamValueToBitString(cursor, ret, chunkSignature);
		sh::streamValueToBitString(cursor, ret, length);
		for (uint32_t i = 0; i < events.size(); i++) {
			sh::streamObjectToBitString(cursor, ret, events[i], events[i]->getLength());
		}
		return ret;
	}

	uint32_t TrackChunk::getLength() {
		return sizeof(chunkSignature) + sizeof(length) + length;
	}

	//---------------------END OF TRACKCHUNK CLASS---------------------//

	HeaderChunk::HeaderChunk(uint32_t length, uint16_t format, uint16_t trackCount, uint16_t tickDivision) {
		chunkSignature = c::MIDI_SIGNATURE;
		this->length = length;
		this->format = format;
		this->trackCount = trackCount;
		this->tickDivision = tickDivision;
	}
	
	char* HeaderChunk::toBitString() {
		char* ret = new char[getLength()];
		uint32_t cursor = 0;
		sh::streamValueToBitString(cursor, ret, chunkSignature);
		sh::streamValueToBitString(cursor, ret, length);
		sh::streamValueToBitString(cursor, ret, format);
		sh::streamValueToBitString(cursor, ret, trackCount);
		sh::streamValueToBitString(cursor, ret, tickDivision);
		return ret;
	}

	uint32_t HeaderChunk::getLength() {
		return sizeof(chunkSignature) + sizeof(length) + length;
	}
	
	//---------------------END OF HEADERCHUNK CLASS---------------------//

	MetaEvent::MetaEvent(VariableLengthValue timeDelta, uint8_t metaEventType, VariableLengthValue eventLength, std::vector<uint8_t> eventData) {
		this->timeDelta = timeDelta;
		this->eventType = 0xFF;
		this->eventData = eventData;
		this->metaEventType = metaEventType;
		this->eventLength = eventLength;
	}

	/*
	MetaEvent::MetaEvent(const Event& other) {
		timeDelta = 0;
		eventType = 0xFF;
		eventData = other.eventData;
	}
	*/

	char* MetaEvent::toBitString() {
		char* ret = new char[getLength()];
		uint32_t cursor = 0;
		sh::streamObjectToBitString(cursor, ret, timeDelta, timeDelta.getLength());
		sh::streamValueToBitString(cursor, ret, eventType);
		sh::streamValueToBitString(cursor, ret, metaEventType);
		sh::streamObjectToBitString(cursor, ret, eventLength, eventLength.getLength());
		for (uint8_t byte : eventData) {
			sh::streamValueToBitString(cursor, ret, byte);
		}
		return ret;
	}

	uint32_t MetaEvent::getLength() {
		return uint32_t(timeDelta.getLength() + sizeof(eventType) + sizeof(metaEventType) + eventLength.getLength() + eventData.size());
	}

	//---------------------END OF METAEVENT CLASS---------------------//

	SysexEvent::SysexEvent(VariableLengthValue timeDelta, uint8_t eventType, VariableLengthValue eventLength, std::vector<uint8_t> eventData) {
		this->timeDelta = timeDelta;
		this->eventType = eventType;
		this->eventData = eventData;
		this->eventLength = eventLength;
	}

	char* SysexEvent::toBitString() {
		char* ret = new char[getLength()];
		uint32_t cursor = 0;
		sh::streamObjectToBitString(cursor, ret, timeDelta, timeDelta.getLength());
		sh::streamValueToBitString(cursor, ret, eventType);
		sh::streamObjectToBitString(cursor, ret, eventLength, eventLength.getLength());
		for (uint8_t byte : eventData) {
			sh::streamValueToBitString(cursor, ret, byte);
		}
		return ret;
	}

	uint32_t SysexEvent::getLength() {
		return uint32_t(timeDelta.getLength() + sizeof(eventType) + eventLength.getLength() + eventData.size());
	}

	//---------------------END OF SYSEXEVENT CLASS---------------------//

	MidiEvent::MidiEvent(VariableLengthValue timeDelta, uint8_t eventType, std::vector<uint8_t> eventData) {
		this->timeDelta = timeDelta;
		this->eventType = eventType;
		this->eventData = eventData;
	}

	char* MidiEvent::toBitString() {
		char* ret = new char[getLength()];
		uint32_t cursor = 0;
		sh::streamObjectToBitString(cursor, ret, timeDelta, timeDelta.getLength());
		sh::streamValueToBitString(cursor, ret, eventType);
		for (uint8_t byte : eventData) {
			sh::streamValueToBitString(cursor, ret, byte);
		}
		return ret;
	}

	uint32_t MidiEvent::getLength() {
		return uint32_t(timeDelta.getLength() + sizeof(eventType) + eventData.size());
	}

	//---------------------END OF MIDIEVENT CLASS---------------------//

	Midi::Midi(std::vector<Chunk*> chunks) {
		this->chunks = chunks;
		if (this->chunks.size() > 0) assert(this->chunks[0]->chunkSignature == c::MIDI_SIGNATURE);
	}

	char* Midi::toBitString() {
		uint32_t counter = 0;
		char* dataStreamString = new char[getLength()];
		for (uint32_t i = 0; i < chunks.size(); i++) {
			sh::streamObjectToBitString(counter, dataStreamString, chunks[i], chunks[i]->getLength());
		}
		return dataStreamString;
	}

	uint32_t Midi::getLength() {
		uint32_t length = 0;
		for (uint32_t i = 0; i < chunks.size(); i++) {
			length += chunks[i]->getLength();
		}
		return length;
	}

	void Midi::fromBitString(const char* bitString, uint32_t fileSize) {
		chunks.clear();
		uint32_t cursor = 0;
		uint32_t signature = sh::getIntFromBitString(cursor, bitString);
		assert(signature == c::MIDI_SIGNATURE);
		uint32_t length = sh::getIntFromBitString(cursor, bitString);
		uint16_t format = sh::getShortFromBitString(cursor, bitString);
		uint16_t tracks = sh::getShortFromBitString(cursor, bitString);
		uint16_t division = sh::getShortFromBitString(cursor, bitString);
		chunks.push_back(new HeaderChunk(length, format, tracks, division));
		while (cursor < fileSize) {
			signature = sh::getIntFromBitString(cursor, bitString);
			assert(signature == c::TRACK_SIGNATURE);
			length = sh::getIntFromBitString(cursor, bitString);
			uint32_t endOfTrack = length + cursor;
			std::vector<Event*> events;
			while (cursor < endOfTrack) {
				vlv deltaTime = sh::getVLVFromBitString(cursor, bitString);
				uint8_t eventType = sh::getCharFromBitString(cursor, bitString);
				if (eventType == c::META_EVENT) {
					uint8_t metaEventType = sh::getCharFromBitString(cursor, bitString);
					if (metaEventType == c::END_OF_TRACK) assert(cursor + 1 == endOfTrack);
					vlv eventLength = sh::getVLVFromBitString(cursor, bitString);
					std::vector<uint8_t> eventData = std::vector<uint8_t>(0);
					if (eventLength.toNumber() != 0) {
						for (uint32_t i = 0; i < eventLength.toNumber(); i++) {
							eventData.push_back(sh::getCharFromBitString(cursor, bitString));
						}
					}
					events.push_back(new MetaEvent(deltaTime, metaEventType, eventLength, eventData));
				}
				else if (eventType < c::META_EVENT && eventType >= c::SYSEX_EVENT) {
					vlv eventLength = sh::getVLVFromBitString(cursor, bitString);
					std::vector<uint8_t> eventData = std::vector<uint8_t>(0);
					if (eventLength.toNumber() != 0) {
						for (uint32_t i = 0; i < eventLength.toNumber(); i++) {
							eventData.push_back(sh::getCharFromBitString(cursor, bitString));
						}
					}
					events.push_back(new SysexEvent(deltaTime, eventType, eventLength, eventData));
				}
				else if (eventType < c::SYSEX_EVENT && eventType >= c::NOTE_OFF) {
					std::vector<uint8_t> eventData = std::vector<uint8_t>(0);
					if (eventType < c::PROGRAM || eventType >= c::PITCH) {
						eventData.push_back(sh::getCharFromBitString(cursor, bitString));
						eventData.push_back(sh::getCharFromBitString(cursor, bitString));
					}
					else {
						eventData.push_back(sh::getCharFromBitString(cursor, bitString));
					}
					events.push_back(new MidiEvent(deltaTime, eventType, eventData));
				}
			}
			chunks.push_back(new TrackChunk(length, events));
		}
	}

	//---------------------END OF MIDI CLASS---------------------//

	bool File::read(const char* filename, Midi& midi) {
		std::ifstream fin(filename, std::ios::binary);
		if (fin.fail()) {
			return false;
		}
		char temp;
		uint32_t filesize = 0;
		while (!fin.eof()) {
			temp = fin.get();
			filesize++;
		}
		filesize--;
		fin.close();
		fin.open(filename, std::ios::binary);
		char* fileData = new char[filesize];
		fin.read(fileData, filesize);
		midi.fromBitString(fileData, filesize);
		return true;
	}

	bool File::write(const char* filename, Midi midi) {
		std::ofstream fout(filename, std::ios::binary);
		if (fout.fail()) {
			return false;
		}
		uint32_t filesize = midi.getLength();
		fout.write(midi.toBitString(), filesize);
		fout.close();
		return true;
	}

	//---------------------END OF FILE CLASS---------------------//

	template<typename T>
	std::string Formatter::formatEvent(T eventIn) {
		std::stringstream retStream;
		if (eventIn->eventType == c::META_EVENT) {
			MetaEvent* eventa = reinterpret_cast<MetaEvent*>(eventa);
			switch (eventa->metaEventType) {
			case 0x00:
				retStream << "Sequence Number";
				uint16_t seqNum = eventa->eventData[0];
				seqNum <<= 8;
				seqNum |= eventa->eventData[1];
				retStream << " " << seqNum << ",";
				break;
			case 0x01:
				retStream << "Text: ";
				for each(uint8_t temp in eventa->eventData) {
					retStream << temp;
				}
				retStream << ",";
				break;
			case 0x02:
				retStream << "Copyright: ";
				for each (uint8_t temp in eventa->eventData) {
					retStream << temp;
				}
				retStream << ",";
				break;
			case 0x03:
				retStream << "TrackName: ";
				for each(uint8_t temp in eventa->eventData) {
					retStream << temp;
				}
				retStream << ",";
				break;
			case 0x04:
				retStream << "Instrument: ";
				for each(uint8_t temp in eventa->eventData) {
					retStream << temp;
				}
				retStream << ",";
				break;
			case 0x05:
				retStream << "Lyric: ";
				for each(uint8_t temp in eventa->eventData) {
					retStream << temp;
				}
				retStream << ",";
				break;
			case 0x06:
				retStream << "Mark: ";
				for each(uint8_t temp in eventa->eventData) {
					retStream << temp;
				}
				retStream << ",";
				break;
			case 0x07:
				retStream << "Cue: ";
				for each(uint8_t temp in eventa->eventData) {
					retStream << temp;
				}
				retStream << ",";
				break;
			case 0x20:
				retStream << "Channel " << (int)eventa->eventData[0] + 1 << ",";
				break;
			case 0x2F:
				retStream << "END OF TRACK,";
				break;
			case 0x51:
				retStream << "Tempo change: ";
				uint32_t tempo = eventa->eventData[0];
				tempo <<= 8;
				tempo |= eventa->eventData[1];
				tempo <<= 8;
				tempo |= eventa->eventData[2];
				tempo = (uint32_t)((1.0F / ((float)tempo / 60.0F)) * 1e+6);
				retStream << tempo << " bpm,";
				break;
			case 0x54:
				retStream << "SMTPE Offset: " << (uint32_t)eventa->eventData[0] << "." << (uint32_t)eventa->eventData[1] << "." << (uint32_t)eventa->eventData[2] << "." << (uint32_t)eventa->eventData[3] << "." << (uint32_t)eventa->eventData[4] << ",";
				break;
			case 0x58:
				retStream << "Time Signature: " << (uint32_t)eventa->eventData[0] << "/" << (0x01 << (uint32_t)eventa->eventData[1]) << ", " << (uint32_t)eventa->eventData[2] << " clocks per tick, " << (uint32_t)eventa->eventData[3] << " 32nd notes per 24 clocks,";
				break;
			case 0x59:
				retStream << "Key Signature: ";
				switch (eventa->eventData[1]) {
				case 0:
					switch ((char)eventa->eventData[0]) {
					case -7:
						retStream << "Cb Major,";
						break;
					case -6:
						retStream << "Gb Major,";
						break;
					case -5:
						retStream << "Db Major,";
						break;
					case -4:
						retStream << "Ab Major,";
						break;
					case -3:
						retStream << "Eb Major,";
						break;
					case -2:
						retStream << "Bb Major,";
						break;
					case -1:
						retStream << "F Major,";
						break;
					case 0:
						retStream << "C Major,";
						break;
					case 1:
						retStream << "G Major,";
						break;
					case 2:
						retStream << "D Major,";
						break;
					case 3:
						retStream << "A Major,";
						break;
					case 4:
						retStream << "E Major,";
						break;
					case 5:
						retStream << "B Major,";
						break;
					case 6:
						retStream << "F# Major,";
						break;
					case 7:
						retStream << "C# Major,";
						break;
					}
					break;
				case 1:
					switch ((char)eventa->eventData[0]) {
					case -7:
						retStream << "Ab minor,";
						break;
					case -6:
						retStream << "Eb minor,";
						break;
					case -5:
						retStream << "Bb minor,";
						break;
					case -4:
						retStream << "F minor,";
						break;
					case -3:
						retStream << "C minor,";
						break;
					case -2:
						retStream << "G minor,";
						break;
					case -1:
						retStream << "D minor,";
						break;
					case 0:
						retStream << "A minor,";
						break;
					case 1:
						retStream << "E minor,";
						break;
					case 2:
						retStream << "B minor,";
						break;
					case 3:
						retStream << "F# minor,";
						break;
					case 4:
						retStream << "C# minor,";
						break;
					case 5:
						retStream << "G# minor,";
						break;
					case 6:
						retStream << "D# minor,";
						break;
					case 7:
						retStream << "A# minor,";
						break;

					};
					break;
				};
				break;
			case 0x7F:
				retStream << "Sequencer Specific event,";
				break;
			};
		}
		else if (eventIn->eventType >= c::SYSEX_EVENT) {
			retStream << eventIn->timeDelta.toNumber() << ",";
			retStream << "System Exclusive Event,";
		}
		else if (eventIn->eventType >= c::PITCH) {
			retStream << eventIn->timeDelta.toNumber() << ",";
		}
		else if (eventIn->eventType >= c::CHANNEL_TOUCH) {
			retStream << eventIn->timeDelta.toNumber() << ",";
		}
		else if (eventIn->eventType >= c::PROGRAM) {
			retStream << eventIn->timeDelta.toNumber() << ",";

		}
		else if (eventIn->eventType >= c::CONTROL) {
			retStream << eventIn->timeDelta.toNumber() << ",";

		}
		else if (eventIn->eventType >= c::POLY_TOUCH) {
			retStream << eventIn->timeDelta.toNumber() << ",";

		}
		else if (eventIn->eventType >= c::NOTE_ON) {
			retStream << eventIn->timeDelta.toNumber() << ",";

		}
		else if (eventIn->eventType >= c::NOTE_OFF) {
			retStream << eventIn->timeDelta.toNumber() << ",";

		}
		return retStream.str();
	}

	template<typename T>
	std::string Formatter::formatChunk(T chunkIn) {
		std::stringstream retStream;
		for (Event* e : chunkIn->events) {
			retStream << formatEvent(e);
		}
		return retStream.str();
	}

	std::string Formatter::formatMidi(Midi midiIn) {
		std::stringstream retStream;
		for (Chunk* c : midiIn.chunks) {
			retStream << formatChunk(c);
		}
		return retStream.str();
	}

	//---------------------END OF FORMATTER CLASS---------------------//
}