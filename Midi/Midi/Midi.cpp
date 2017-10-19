#include "Midi.h"

namespace mid {
	Midi::Midi(std::vector<Chunk> chunks) {
		this->chunks = chunks;
		if (this->chunks.size() > 0) assert(this->chunks[0].chunkSignature == MIDI_SIGNATURE);
	}
	Midi::Midi(const Midi& other) {
		chunks = other.chunks;
	}

	char* Midi::toBitString() {
		uint filesize = 0;
		for (Chunk chunk : chunks) {
			filesize += sizeof(chunk.chunkSignature) + sizeof(chunk.length) + chunk.length;
		}
		char* dataStreamString = new char[filesize];
		return dataStreamString;
	}

	void Midi::fromBitString(const char* bitString) {
		//I DUNNO!!!!
	}
}