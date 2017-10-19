#include "Midi.h"

namespace mid {
	Midi::Midi(std::vector<Chunk*> chunks) {
		this->chunks = chunks;
		if (this->chunks.size() > 0) assert(this->chunks[0]->chunkSignature == MIDI_SIGNATURE);
	}
	Midi::Midi(const Midi& other) {
		chunks = other.chunks;
	}

	char* Midi::toBitString() {
		uint filesize = 0;
		uint counter = 0;
		for (uint i = 0; i < chunks.size(); i++){
			filesize += (sizeof(chunks[i]->chunkSignature) + sizeof(chunks[i]->length) + chunks[i]->length);
		}
		char* dataStreamString = new char[filesize];
		for (uint i = 0; i < chunks.size(); i++){
			sh::streamObjectToBitString(counter, dataStreamString, chunks[i], chunks[i]->getLength());
		}
		return dataStreamString;
	}

	void Midi::fromBitString(const char* bitString) {
		//I DUNNO!!!!
	}
}