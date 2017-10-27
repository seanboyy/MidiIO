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

	template<typename T>
	void StreamingHelper::streamObjectToBitString(uint& cursor, char*& bitString, T* object, uint length) {
		char* objStr = object->toBitString();
		for (uint i = 0; i < length; i++) {
			bitString[cursor] = objStr[i];
			cursor++;
		}
	}

	uchar StreamingHelper::getCharFromBitString(uint& cursor, const char* bitString) {
		return bitString[cursor++];
	}

	ushort StreamingHelper::getShortFromBitString(uint& cursor, const char* bitString) {
		ushort result = (uchar)bitString[cursor++];
		result <<= 8;
		result |= (uchar)bitString[cursor++];
		return result;
	}

	uint StreamingHelper::getIntFromBitString(uint& cursor, const char* bitString) {
		uint result = (uchar)bitString[cursor++];
		result <<= 8;
		result = (result | (uchar)bitString[cursor++]) << 8;
		result = (result | (uchar)bitString[cursor++]) << 8;
		result |= (uchar)bitString[cursor++];
		return result;
	}

	VariableLengthValue StreamingHelper::getVLVFromBitString(uint& cursor, const char* bitString) {
		uint length = 1;
		uint value = 0;
		for (uint i = 0; i < length; i++) {
			uchar tempByte = getCharFromBitString(cursor, bitString);
			if ((tempByte & 0x80) == 0x80) length++;
			value |= tempByte;
			if (!(i == length - 1)) value <<= 8;
		}
		return VariableLengthValue(VariableLengthValue::toNumber(value));
	}

	//---------------------END OF STREAMINGHELPER CLASS---------------------//


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
			result |= b4;
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

	//---------------------END OF VARIABLELENGTHVALUE CLASS---------------------//

	TrackChunk::TrackChunk(uint length, std::vector<Event*> events) {
		this->chunkSignature = c::TRACK_SIGNATURE;
		this->length = length;
		this->events = events;
	}

	/*
	TrackChunk::TrackChunk(const Chunk* other) {
		this->chunkSignature = c::TRACK_SIGNATURE;
		this->length = other->length;
		for each(Event* event in other->events) {

		}
	}

	TrackChunk::TrackChunk(const Chunk& other) {
		this->chunkSignature = c::TRACK_SIGNATURE;
		this->length = other.length;
		for each(Event* event in other.events) {

		}
	}

	TrackChunk::TrackChunk(const TrackChunk& other) {
		chunkSignature = c::TRACK_SIGNATURE;
		length = other.length;
		events.clear();
		for each(Event* event in other.events) {
			if (event->eventType == c::META_EVENT) events.push_back(new MetaEvent(*event));
			else if (event->eventType >= c::SYSEX_EVENT) events.push_back(new SysexEvent(*event));
			else if (event->eventType >= c::NOTE_OFF && event->eventType < c::SYSEX_EVENT) events.push_back(new MidiEvent(*event));
		}
	}
	*/

	char* TrackChunk::toBitString() {
		char* ret = new char[getLength()];
		uint cursor = 0;
		sh::streamValueToBitString(cursor, ret, chunkSignature);
		sh::streamValueToBitString(cursor, ret, length);
		for (uint i = 0; i < events.size(); i++) {
			sh::streamObjectToBitString(cursor, ret, events[i], events[i]->getLength());
		}
		return ret;
	}

	uint TrackChunk::getLength() {
		return sizeof(chunkSignature) + sizeof(length) + length;
	}

	//---------------------END OF TRACKCHUNK CLASS---------------------//

	HeaderChunk::HeaderChunk(uint length, ushort format, ushort trackCount, ushort tickDivision) {
		chunkSignature = c::MIDI_SIGNATURE;
		this->length = length;
		this->format = format;
		this->trackCount = trackCount;
		this->tickDivision = tickDivision;
	}
	
	/*
	HeaderChunk::HeaderChunk(const Chunk* other) {
		chunkSignature = c::MIDI_SIGNATURE;
		this->length = 6;
		this->format = 0;
		this->trackCount = 1;
		this->tickDivision = 192;
	}

	HeaderChunk::HeaderChunk(const Chunk& other) {
		chunkSignature = c::MIDI_SIGNATURE;
		this->length = 6;
		this->format = 0;
		this->trackCount = 1;
		this->tickDivision = 192;
	}

	HeaderChunk::HeaderChunk(const HeaderChunk& other) {
		chunkSignature = c::MIDI_SIGNATURE;
		length = 6;
		format = other.format;
		trackCount = other.trackCount;
		tickDivision = other.tickDivision;
	}
	*/

	char* HeaderChunk::toBitString() {
		char* ret = new char[getLength()];
		uint cursor = 0;
		sh::streamValueToBitString(cursor, ret, chunkSignature);
		sh::streamValueToBitString(cursor, ret, length);
		sh::streamValueToBitString(cursor, ret, format);
		sh::streamValueToBitString(cursor, ret, trackCount);
		sh::streamValueToBitString(cursor, ret, tickDivision);
		return ret;
	}

	uint HeaderChunk::getLength() {
		return sizeof(chunkSignature) + sizeof(length) + length;
	}
	
	//---------------------END OF HEADERCHUNK CLASS---------------------//

	MetaEvent::MetaEvent(VariableLengthValue timeDelta, uchar metaEventType, VariableLengthValue eventLength, std::vector<uchar> eventData) {
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

	uint MetaEvent::getLength() {
		return timeDelta.getLength() + sizeof(eventType) + sizeof(metaEventType) + eventLength.getLength() + eventData.size();
	}

	//---------------------END OF METAEVENT CLASS---------------------//

	SysexEvent::SysexEvent(VariableLengthValue timeDelta, uchar eventType, VariableLengthValue eventLength, std::vector<uchar> eventData) {
		this->timeDelta = timeDelta;
		this->eventType = eventType;
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

	//---------------------END OF MIDIEVENT CLASS---------------------//

	Midi::Midi(std::vector<Chunk*> chunks) {
		this->chunks = chunks;
		if (this->chunks.size() > 0) assert(this->chunks[0]->chunkSignature == c::MIDI_SIGNATURE);
	}

	/*
	Midi::Midi(const Midi& other) {
		this->chunks.clear();
		for each(Chunk* chunk in other.chunks) {
			if (chunk->chunkSignature == c::MIDI_SIGNATURE) this->chunks.push_back(new HeaderChunk(*chunk));
			if (chunk->chunkSignature == c::TRACK_SIGNATURE) this->chunks.push_back(new TrackChunk(*chunk));
		}
	}

	Midi::~Midi() {}
	*/

	char* Midi::toBitString() {
		uint counter = 0;
		char* dataStreamString = new char[getLength()];
		for (uint i = 0; i < chunks.size(); i++) {
			sh::streamObjectToBitString(counter, dataStreamString, chunks[i], chunks[i]->getLength());
		}
		return dataStreamString;
	}

	uint Midi::getLength() {
		uint length = 0;
		for (uint i = 0; i < chunks.size(); i++) {
			length += chunks[i]->getLength();
		}
		return length;
	}

	void Midi::fromBitString(const char* bitString, uint fileSize) {
		chunks.clear();
		uint cursor = 0;
		uint signature = sh::getIntFromBitString(cursor, bitString);
		assert(signature == c::MIDI_SIGNATURE);
		uint length = sh::getIntFromBitString(cursor, bitString);
		ushort format = sh::getShortFromBitString(cursor, bitString);
		ushort tracks = sh::getShortFromBitString(cursor, bitString);
		ushort division = sh::getShortFromBitString(cursor, bitString);
		chunks.push_back(new HeaderChunk(length, format, tracks, division));
		while (cursor < fileSize) {
			signature = sh::getIntFromBitString(cursor, bitString);
			assert(signature == c::TRACK_SIGNATURE);
			length = sh::getIntFromBitString(cursor, bitString);
			uint endOfTrack = length + cursor;
			std::vector<Event*> events;
			while (cursor < endOfTrack) {
				vlv deltaTime = sh::getVLVFromBitString(cursor, bitString);
				uchar eventType = sh::getCharFromBitString(cursor, bitString);
				if (eventType == c::META_EVENT) {
					uchar metaEventType = sh::getCharFromBitString(cursor, bitString);
					if (metaEventType == c::END_OF_TRACK) assert(cursor + 1 == endOfTrack);
					vlv eventLength = sh::getVLVFromBitString(cursor, bitString);
					std::vector<uchar> eventData = std::vector<uchar>(0);
					if (eventLength.toNumber() != 0) {
						for (uint i = 0; i < eventLength.toNumber(); i++) {
							eventData.push_back(sh::getCharFromBitString(cursor, bitString));
						}
					}
					events.push_back(new MetaEvent(deltaTime, metaEventType, eventLength, eventData));
				}
				else if (eventType < c::META_EVENT && eventType >= c::SYSEX_EVENT) {
					vlv eventLength = sh::getVLVFromBitString(cursor, bitString);
					std::vector<uchar> eventData = std::vector<uchar>(0);
					if (eventLength.toNumber() != 0) {
						for (uint i = 0; i < eventLength.toNumber(); i++) {
							eventData.push_back(sh::getCharFromBitString(cursor, bitString));
						}
					}
					events.push_back(new SysexEvent(deltaTime, eventType, eventLength, eventData));
				}
				else if (eventType < c::SYSEX_EVENT && eventType >= c::NOTE_OFF) {
					std::vector<uchar> eventData = std::vector<uchar>(0);
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
		uint filesize = 0;
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
		uint filesize = midi.getLength();
		fout.write(midi.toBitString(), filesize);
		fout.close();
		return true;
	}

	//---------------------END OF FILE CLASS---------------------//
}