#pragma once

#include <vector>
#include <cassert>
#include <iostream>

namespace mid {

	typedef unsigned char uchar;
	typedef unsigned int uint;
	typedef unsigned short ushort;
	typedef unsigned long ulong;
	typedef unsigned long long ullong;

	const static uint MIDI_SIGNATURE = 0x4D546864;
	const static uint TRACK_SIGNATURE = 0x4D54726B;

	typedef class StreamingHelper {
	public:
		template<typename T>
		static void streamValueToBitString(uint&, char*&, T);

		template<typename T>
		static void streamObjectToBitString(uint&, char*&, T, uint);
	}sh, streamHelp, streamhelp;

	typedef class VariableLengthValue {
	public:
		const static uint VLV_MAX_IN = 0x0FFFFFFF;
		const static uint VLV_MAX_VAL = 0xFFFFFF7F;
		VariableLengthValue(uint = 0);
		VariableLengthValue(const VariableLengthValue&);
		uint toNumber();
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
		Event(VariableLengthValue timeDelta = VariableLengthValue(), uchar eventType = 0x00, std::vector<uchar> eventData = std::vector<uchar>(0));
		Event(const Event&);
		char* toBitString();
		uint getLength();
	protected:
		VariableLengthValue timeDelta;
		uchar eventType;
		std::vector<uchar> eventData;
	};

	class MetaEvent : Event {
	public:
		MetaEvent(VariableLengthValue timeDelta = VariableLengthValue(), uchar metaEventType = 0x00, VariableLengthValue eventLength = VariableLengthValue(), std::vector<uchar> eventData = std::vector<uchar>(0));
		char* toBitString();
		uint getLength();
	private:
		uchar metaEventType;
		VariableLengthValue eventLength;
	};

	class SysexEvent : Event {
	public:
		SysexEvent(VariableLengthValue timeDelta = VariableLengthValue(), uchar eventType = 0xF0, VariableLengthValue eventLength = VariableLengthValue(), std::vector<uchar> eventData = std::vector<uchar>(0));
		char* toBitString();
		uint getLength();
	private:
		VariableLengthValue eventLength;
	};

	class MidiEvent : Event {
	public:
		MidiEvent(VariableLengthValue timeDelta = VariableLengthValue(), uchar eventType = 0x80, std::vector<uchar> eventData = std::vector<uchar>(0));
		char* toBitString();
		uint getLength();
	};

	class Chunk {
	public:
		Chunk(uint chunkSignature = TRACK_SIGNATURE, uint length = 0, std::vector<Event> events = std::vector<Event>(0));
		uint getLength();
		char* toBitString();
		uint chunkSignature;
		uint length;
		std::vector<Event> events;
	};

	class HeaderChunk : Chunk {
	public:
		HeaderChunk(uint length = 6, ushort format = 0, ushort trackCount = 1, ushort tickDivision = 0x00C0);
		char* toBitString();
	private:
		ushort format;
		ushort trackCount;
		ushort tickDivision;
	};

	class Midi {
	public:
		Midi(std::vector<Chunk> chunks = std::vector<Chunk>(0));
		Midi(const Midi&);
		char* toBitString();
		void fromBitString(const char*);
	private:
		std::vector<Chunk> chunks;
	};

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
}