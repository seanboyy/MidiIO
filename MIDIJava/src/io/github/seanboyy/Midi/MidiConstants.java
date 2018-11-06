package io.github.seanboyy.Midi;

class MidiConstants{
	static final int MIDI_SIGNATURE = 0x4D546864;
	static final int TRACK_SIGNATURE = 0x4D54726B;
	static final byte META_EVENT = (byte)0xFF;
	static final byte SYSEX_EVENT = (byte)0xF0;
	static final byte END_OF_TRACK = (byte)0x2F;
	static final byte NOTE_OFF = (byte)0x80;
	static final byte NOTE_ON = (byte)0x90;
	static final byte POLY_TOUCH = (byte)0xA0;
	static final byte CONTROL = (byte)0xB0;
	static final byte PROGRAM = (byte)0xC0;
	static final byte CHANNEL_TOUCH = (byte)0xD0;
	static final byte PITCH = (byte)0xE0;
	static final byte C4 = (byte)0x3C;
}
