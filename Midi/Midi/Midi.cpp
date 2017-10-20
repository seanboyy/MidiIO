#include "Midi.h"

namespace mid {
	Midi::Midi(std::vector<Chunk*> chunks) {
		this->chunks = chunks;
		if (this->chunks.size() > 0) assert(this->chunks[0]->chunkSignature == c::MIDI_SIGNATURE);
	}
	Midi::Midi(const Midi& other) {
		chunks = other.chunks;
	}

	char* Midi::toBitString() {
		uint counter = 0;
		char* dataStreamString = new char[getLength()];
		for (uint i = 0; i < chunks.size(); i++){
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

	void Midi::fromBitString(const char* bitString) {
		
	}
}