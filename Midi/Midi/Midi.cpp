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

	void Midi::fromBitString(const char* bitString, uint fileSize) {
		chunks.clear();
		uint cursor = 0;
		uint signature1 = sh::getIntFromBitString(cursor, bitString);
		assert(signature1 == c::MIDI_SIGNATURE);
		uint length1 = sh::getIntFromBitString(cursor, bitString);
		ushort format1 = sh::getShortFromBitString(cursor, bitString);
		ushort tracks1 = sh::getShortFromBitString(cursor, bitString);
		ushort division1 = sh::getShortFromBitString(cursor, bitString);
		Chunk* head = &HeaderChunk(length1, format1, tracks1, division1);
		chunks.push_back(head);
		while (cursor < fileSize) {
			uint signature = sh::getIntFromBitString(cursor, bitString);
			assert(signature == c::TRACK_SIGNATURE);
			uint length = sh::getIntFromBitString(cursor, bitString);
			uint endOfTrack = length + cursor;
			std::vector<Event*> events;
			while (cursor < endOfTrack) {
				vlv deltaTime = sh::getVLVFromBitString(cursor, bitString);
				uchar eventType = sh::getCharFromBitString(cursor, bitString);
				if (eventType == c::META_EVENT) {
					uchar metaEventType = sh::getCharFromBitString(cursor, bitString);
					if (metaEventType = c::END_OF_TRACK) assert(cursor + 1 == endOfTrack);
					vlv length = sh::getVLVFromBitString(cursor, bitString);
					std::vector<uchar> eventData = std::vector<uchar>(0);
					if (length != 0) {
						for (int i = 0; i < length; i++) {
							eventData.push_back(sh::getCharFromBitString(cursor, bitString));
						}
					}
					events.push_back(new MetaEvent(deltaTime, metaEventType, length, eventData));
				}
				else if (eventType < c::META_EVENT && eventType >= c::SYSEX_EVENT) {
					vlv length = sh::getVLVFromBitString(cursor, bitString);
					std::vector<uchar> eventData = std::vector<uchar>(0);
					if (length != 0) {
						for (int i = 0; i < length; i++) {
							eventData.push_back(sh::getCharFromBitString(cursor, bitString));
						}
					}
					events.push_back(new SysexEvent(deltaTime, eventType, length, eventData));
				}
				else if (eventType < c::SYSEX_EVENT && eventType >= c::NOTE_OFF) {
					std::vector<uchar> eventData = std::vector<uchar>(0);
					if (eventType < c::PROGRAM || eventType >= c::PITCH) {
						for (int i = 0; i < 2; i++) {
							eventData.push_back(sh::getCharFromBitString(cursor, bitString));
						}
					}
					else {
						eventData.push_back(sh::getCharFromBitString(cursor, bitString));
					}
					events.push_back(new MidiEvent(deltaTime, eventType, eventData));
				}
			}
		}

	}
}