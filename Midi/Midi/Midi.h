#pragma once

#include <vector>
#include <cassert>
#include <iostream>
#include <fstream>

namespace mid {

	typedef unsigned char uchar;
	typedef unsigned int uint;
	typedef unsigned short ushort;
	typedef unsigned long ulong;
	typedef unsigned long long ullong;

	typedef class StreamingHelper {
	public:
		template<typename T>
		static void streamValueToBitString(uint&, char*&, T);
		template<typename T>
		static void streamObjectToBitString(uint&, char*&, T, uint);
		template<typename T>
		static void streamObjectToBitString(uint&, char*&, T*, uint);
		static uchar getCharFromBitString(uint&, const char*);
		static ushort getShortFromBitString(uint&, const char*);
		static uint getIntFromBitString(uint&, const char*);
		static VariableLengthValue getVLVFromBitString(uint&, const char*);
	}sh, streamHelp, streamhelp;

	typedef class VariableLengthValue {
	public:
		const static uint VLV_MAX_IN = 0x0FFFFFFF;
		const static uint VLV_MAX_VAL = 0xFFFFFF7F;
		VariableLengthValue(uint = 0);
		VariableLengthValue(const VariableLengthValue&);
		uint toNumber();
		uint toNumber(uint);
		void toVariableLength(uint);
		char* toBitString();
		uint getLength();
		operator uint();
		void operator= (uint);
		void operator= (int);
		void operator= (const VariableLengthValue&);
		friend void operator! (VariableLengthValue&);
		friend bool operator== (VariableLengthValue&, uint);
		friend bool operator== (VariableLengthValue&, int);
		friend bool operator!= (VariableLengthValue&, uint);
		friend bool operator!= (VariableLengthValue&, int);
		friend bool operator< (VariableLengthValue&, uint);
		friend bool operator< (VariableLengthValue&, int);
		friend bool operator> (VariableLengthValue&, uint);
		friend bool operator> (VariableLengthValue&, int);
		friend bool operator<= (VariableLengthValue&, uint);
		friend bool operator<= (VariableLengthValue&, int);
		friend bool operator>= (VariableLengthValue&, uint);
		friend bool operator>= (VariableLengthValue&, int);
		friend VariableLengthValue operator+ (VariableLengthValue&, uint);
		friend VariableLengthValue operator+ (VariableLengthValue&, int);
		friend VariableLengthValue operator- (VariableLengthValue&, uint);
		friend VariableLengthValue operator- (VariableLengthValue&, int);
		friend VariableLengthValue operator* (VariableLengthValue&, uint);
		friend VariableLengthValue operator* (VariableLengthValue&, int);
		friend VariableLengthValue operator/ (VariableLengthValue&, uint);
		friend VariableLengthValue operator/ (VariableLengthValue&, int);
		friend VariableLengthValue operator<< (VariableLengthValue&, uint);
		friend VariableLengthValue operator<< (VariableLengthValue&, int);
		friend VariableLengthValue& operator+= (VariableLengthValue&, uint);
		friend VariableLengthValue& operator+= (VariableLengthValue&, int);
		friend VariableLengthValue& operator-= (VariableLengthValue&, uint);
		friend VariableLengthValue& operator-= (VariableLengthValue&, int);
		friend VariableLengthValue& operator*= (VariableLengthValue&, uint);
		friend VariableLengthValue& operator*= (VariableLengthValue&, int);
		friend VariableLengthValue& operator/= (VariableLengthValue&, uint);
		friend VariableLengthValue& operator/= (VariableLengthValue&, int);
		friend VariableLengthValue& operator<<= (VariableLengthValue&, uint);
		friend VariableLengthValue& operator<<= (VariableLengthValue&, int);
		friend VariableLengthValue& operator++ (VariableLengthValue&);
		friend VariableLengthValue operator++ (VariableLengthValue&, int);
		friend VariableLengthValue& operator-- (VariableLengthValue&);
		friend VariableLengthValue operator-- (VariableLengthValue&, int);
	private:
		uint length;
		uint value;
	}vlv, varLength, varLen;

	class Event {
	public:
		virtual char* toBitString() = 0;
		virtual uint getLength() = 0;
	protected:
		VariableLengthValue timeDelta;
		uchar eventType;
		std::vector<uchar> eventData;
	};

	class MetaEvent : virtual public Event {
	public:
		MetaEvent(VariableLengthValue timeDelta = VariableLengthValue(), uchar metaEventType = 0x00, VariableLengthValue eventLength = VariableLengthValue(), std::vector<uchar> eventData = std::vector<uchar>(0));
		char* toBitString();
		uint getLength();
	private:
		uchar metaEventType;
		VariableLengthValue eventLength;
	};

	class SysexEvent : virtual public Event {
	public:
		SysexEvent(VariableLengthValue timeDelta = VariableLengthValue(), uchar eventType = 0xF0, VariableLengthValue eventLength = VariableLengthValue(), std::vector<uchar> eventData = std::vector<uchar>(0));
		char* toBitString();
		uint getLength();
	private:
		VariableLengthValue eventLength;
	};

	class MidiEvent : virtual public Event {
	public:
		MidiEvent(VariableLengthValue timeDelta = VariableLengthValue(), uchar eventType = 0x80, std::vector<uchar> eventData = std::vector<uchar>(0));
		char* toBitString();
		uint getLength();
	};

	class Chunk {
	public:
		virtual char* toBitString() = 0;
		virtual uint getLength() = 0;
		uint chunkSignature;
		uint length;
		std::vector<Event*> events;
	};

	class HeaderChunk : virtual public Chunk {
	public:
		HeaderChunk(uint length = 6, ushort format = 0, ushort trackCount = 1, ushort tickDivision = 0x00C0);
		char* toBitString();
		uint getLength();
	private:
		ushort format;
		ushort trackCount;
		ushort tickDivision;
	};

	class TrackChunk : virtual public Chunk {
	public:
		TrackChunk(uint length = 0, std::vector<Event*> events = std::vector<Event*>(0));
		uint getLength();
		char* toBitString();
	};

	class Midi {
	public:
		Midi(std::vector<Chunk*> chunks = std::vector<Chunk*>(0));
		Midi(const Midi&);
		char* toBitString();
		void fromBitString(const char*, uint);
		uint getLength();
	private:
		std::vector<Chunk*> chunks;
	};

	class File {
	public:
		static bool read(const char*, Midi&);
		static bool write(const char*, Midi);
	};

	typedef class MidiConstants {
	public:
		const static uint MIDI_SIGNATURE = 0x4D546864;
		const static uint TRACK_SIGNATURE = 0x4D54726B;
		const static uchar META_EVENT = 0xFF;
		const static uchar SYSEX_EVENT = 0xF0;
		const static uchar END_OF_TRACK = 0x2F;
		const static uchar NOTE_OFF = 0x80;
		const static uchar NOTE_ON = 0x90;
		const static uchar POLY_TOUCH = 0xA0;
		const static uchar CONTROL = 0xB0;
		const static uchar PROGRAM = 0xC0;
		const static uchar CHANNEL_TOUCH = 0xD0;
		const static uchar PITCH = 0xE0;
		const static uchar C4 = 0x3C;
	}constants, c;

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
		ushort result = bitString[cursor++];
		result <<= 8;
		result |= bitString[cursor++];
		return result;
	}

	uint StreamingHelper::getIntFromBitString(uint& cursor, const char* bitString) {
		uint result = bitString[cursor++];
		result <<= 8;
		result = (result | bitString[cursor++]) << 8;
		result = (result | bitString[cursor++]) << 8;
		result |= bitString[cursor++];
	}

	VariableLengthValue StreamingHelper::getVLVFromBitString(uint& cursor, const char* bitString) {
		uint length = 1;
		uint value = 0;
		for (uint i = 0; i < length; i++) {
			uchar tempByte = getCharFromBitString(cursor, bitString);
			if (tempByte & 0x80 == 0x80) length++;
			value |= tempByte;
			if (!(i == length - 1)) value <<= 8;
		}
	}
}
