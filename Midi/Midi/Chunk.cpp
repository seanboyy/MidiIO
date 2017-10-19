#include "Midi.h"

namespace mid {
	Chunk::Chunk(uint chunkSignature, uint length, std::vector<Event> events) {
		this->chunkSignature = chunkSignature;
		assert(chunkSignature == MIDI_SIGNATURE || chunkSignature == TRACK_SIGNATURE);
		this->length = length;
		this->events = events;
	}

	char* Chunk::toBitString() {
		char* ret = new char[getLength()];
		uint cursor = 0;
		sh::streamValueToBitString(cursor, ret, chunkSignature);
		sh::streamValueToBitString(cursor, ret, length);
		for (Event e : events) {
			sh::streamObjectToBitString(cursor, ret, e, e.getLength());
		}
		return ret;
	}

	uint Chunk::getLength() {
		return sizeof(chunkSignature) + sizeof(length) + length;
	}

	HeaderChunk::HeaderChunk(uint length, ushort format, ushort trackCount, ushort tickDivision) {
		chunkSignature = MIDI_SIGNATURE;
		this->length = length;
		this->format = format;
		this->trackCount = trackCount;
		this->tickDivision = tickDivision;
	}

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
}