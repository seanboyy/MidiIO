#pragma once

#include <vector>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace mid {
	typedef class VariableLengthValue {
	public:
		const static uint32_t VLV_MAX_IN = 0x0FFFFFFF;
		const static uint32_t VLV_MAX_VAL = 0xFFFFFF7F;
		VariableLengthValue(uint32_t = 0);
		VariableLengthValue(const VariableLengthValue&);
		uint32_t toNumber();
		static uint32_t toNumber(uint32_t);
		void toVariableLength(uint32_t);
		char* toBitString();
		uint32_t getLength();
		operator uint32_t();
		void operator= (uint32_t);
		void operator= (int);
		void operator= (const VariableLengthValue&);
		friend void operator! (VariableLengthValue&);
		friend bool operator== (VariableLengthValue&, uint32_t);
		friend bool operator== (VariableLengthValue&, int);
		friend bool operator!= (VariableLengthValue&, uint32_t);
		friend bool operator!= (VariableLengthValue&, int);
		friend bool operator< (VariableLengthValue&, uint32_t);
		friend bool operator< (VariableLengthValue&, int);
		friend bool operator> (VariableLengthValue&, uint32_t);
		friend bool operator> (VariableLengthValue&, int);
		friend bool operator<= (VariableLengthValue&, uint32_t);
		friend bool operator<= (VariableLengthValue&, int);
		friend bool operator>= (VariableLengthValue&, uint32_t);
		friend bool operator>= (VariableLengthValue&, int);
		friend VariableLengthValue operator+ (VariableLengthValue&, uint32_t);
		friend VariableLengthValue operator+ (VariableLengthValue&, int);
		friend VariableLengthValue operator- (VariableLengthValue&, uint32_t);
		friend VariableLengthValue operator- (VariableLengthValue&, int);
		friend VariableLengthValue operator* (VariableLengthValue&, uint32_t);
		friend VariableLengthValue operator* (VariableLengthValue&, int);
		friend VariableLengthValue operator/ (VariableLengthValue&, uint32_t);
		friend VariableLengthValue operator/ (VariableLengthValue&, int);
		friend VariableLengthValue operator<< (VariableLengthValue&, uint32_t);
		friend VariableLengthValue operator<< (VariableLengthValue&, int);
		friend VariableLengthValue& operator+= (VariableLengthValue&, uint32_t);
		friend VariableLengthValue& operator+= (VariableLengthValue&, int);
		friend VariableLengthValue& operator-= (VariableLengthValue&, uint32_t);
		friend VariableLengthValue& operator-= (VariableLengthValue&, int);
		friend VariableLengthValue& operator*= (VariableLengthValue&, uint32_t);
		friend VariableLengthValue& operator*= (VariableLengthValue&, int);
		friend VariableLengthValue& operator/= (VariableLengthValue&, uint32_t);
		friend VariableLengthValue& operator/= (VariableLengthValue&, int);
		friend VariableLengthValue& operator<<= (VariableLengthValue&, uint32_t);
		friend VariableLengthValue& operator<<= (VariableLengthValue&, int);
		friend VariableLengthValue& operator++ (VariableLengthValue&);
		friend VariableLengthValue operator++ (VariableLengthValue&, int);
		friend VariableLengthValue& operator-- (VariableLengthValue&);
		friend VariableLengthValue operator-- (VariableLengthValue&, int);
	private:
		uint32_t length;
		uint32_t value;
	}vlv, varLength, varLen;

	typedef class StreamingHelper {
	public:
		static uint8_t getCharFromBitString(uint32_t&, const char*);
		static uint16_t getShortFromBitString(uint32_t&, const char*);
		static uint32_t getIntFromBitString(uint32_t&, const char*);
		static VariableLengthValue getVLVFromBitString(uint32_t&, const char*);

		template<typename T>
		static void streamValueToBitString(uint32_t&, char*&, T);
		template<typename T>
		static void streamObjectToBitString(uint32_t&, char*&, T, uint32_t);
		template<typename T>
		static void streamObjectToBitString(uint32_t&, char*&, T*, uint32_t);
	}sh, streamHelp, streamhelp;

	class Event {
	public:
		virtual char* toBitString() = 0;
		virtual uint32_t getLength() = 0;
		VariableLengthValue timeDelta;
		uint8_t eventType;
		std::vector<uint8_t> eventData;
	};

	class MetaEvent : virtual public Event {
	public:
		MetaEvent(VariableLengthValue timeDelta = VariableLengthValue(), uint8_t metaEventType = 0x00, VariableLengthValue eventLength = VariableLengthValue(), std::vector<uint8_t> eventData = std::vector<uint8_t>(0));
		char* toBitString();
		uint32_t getLength();
		uint8_t metaEventType;
		VariableLengthValue eventLength;
	};

	class SysexEvent : virtual public Event {
	public:
		SysexEvent(VariableLengthValue timeDelta = VariableLengthValue(), uint8_t eventType = 0xF0, VariableLengthValue eventLength = VariableLengthValue(), std::vector<uint8_t> eventData = std::vector<uint8_t>(0));
		char* toBitString();
		uint32_t getLength();
	private:
		VariableLengthValue eventLength;
	};

	class MidiEvent : virtual public Event {
	public:
		MidiEvent(VariableLengthValue timeDelta = VariableLengthValue(), uint8_t eventType = 0x80, std::vector<uint8_t> eventData = std::vector<uint8_t>(0));
		char* toBitString();
		uint32_t getLength();
	};

	class Chunk {
	public:
		virtual char* toBitString() = 0;
		virtual uint32_t getLength() = 0;
		uint32_t chunkSignature;
		uint32_t length;
		std::vector<Event*> events;
	};

	class HeaderChunk : virtual public Chunk {
	public:
		HeaderChunk(uint32_t length = 6, uint16_t format = 0, uint16_t trackCount = 1, uint16_t tickDivision = 0x00C0);
		char* toBitString();
		uint32_t getLength();
	private:
		uint16_t format;
		uint16_t trackCount;
		uint16_t tickDivision;
	};

	class TrackChunk : virtual public Chunk {
	public:
		TrackChunk(uint32_t length = 0, std::vector<Event*> events = std::vector<Event*>(0));
		uint32_t getLength();
		char* toBitString();
	};

	class Midi {
	public:
		Midi(std::vector<Chunk*> chunks = std::vector<Chunk*>(0));
		char* toBitString();
		void fromBitString(const char*, uint32_t);
		uint32_t getLength();
		std::vector<Chunk*> chunks;
	};

	class File {
	public:
		static bool read(const char*, Midi&);
		static bool write(const char*, Midi);
	};

	class Formatter {
	public:
		template<typename T>
		static std::string formatEvent(T);
		template<typename T>
		static std::string formatChunk(T);
		static std::string formatMidi(Midi);
	};

	typedef class MidiConstants {
	public:
		const static uint32_t MIDI_SIGNATURE = 0x4D546864;
		const static uint32_t TRACK_SIGNATURE = 0x4D54726B;
		const static uint8_t META_EVENT = 0xFF;
		const static uint8_t SYSEX_EVENT = 0xF0;
		const static uint8_t END_OF_TRACK = 0x2F;
		const static uint8_t NOTE_OFF = 0x80;
		const static uint8_t NOTE_ON = 0x90;
		const static uint8_t POLY_TOUCH = 0xA0;
		const static uint8_t CONTROL = 0xB0;
		const static uint8_t PROGRAM = 0xC0;
		const static uint8_t CHANNEL_TOUCH = 0xD0;
		const static uint8_t PITCH = 0xE0;
		const static uint8_t C4 = 0x3C;
	}constants, c;
}
