#include "Midi.h"

namespace mid {
	/*
	uint Chunk::getLength() {
		return sizeof(chunkSignature) + sizeof(length) + length;
	}

	char* Chunk::toBitString() {
		char* ret = new char[getLength()];
		uint cursor = 0;
		sh::streamValueToBitString(cursor, ret, chunkSignature);
		sh::streamValueToBitString(cursor, ret, length);
		for (uint i = 0; i < events.size(); i++) {
			sh::streamObjectToBitString(cursor, ret, events[i], events[i].getLength());
		}
		return ret;
	}
	*/

	TrackChunk::TrackChunk(uint length, std::vector<Event*> events) {
		this->chunkSignature = c::TRACK_SIGNATURE;
		this->length = length;
		this->events = events;
	}

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

	HeaderChunk::HeaderChunk(uint length, ushort format, ushort trackCount, ushort tickDivision) {
		chunkSignature = c::MIDI_SIGNATURE;
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

	uint HeaderChunk::getLength() {
		return sizeof(chunkSignature) + sizeof(length) + length;
	}
}