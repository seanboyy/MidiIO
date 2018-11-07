package io.github.seanboyy.Midi;

import java.util.HashMap;
import java.util.Map;

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

	static final Map<Byte, String> NOTE_MAP = new HashMap<>(){
		{
			put((byte)0x00, "B-2");
			put((byte)0x01, "C-1");
			put((byte)0x02, "C#-1");
			put((byte)0x03, "D-1");
			put((byte)0x04, "D#-1");
			put((byte)0x05, "E-1");
			put((byte)0x06, "F-1");
			put((byte)0x07, "F#-1");
			put((byte)0x08, "G-1");
			put((byte)0x09, "G#-1");
			put((byte)0x0A, "A-1");
			put((byte)0x0B, "A#-1");
			put((byte)0x0C, "B-1");
			put((byte)0x0D, "C0");
			put((byte)0x0E, "C#0");
			put((byte)0x0F, "D0");
			put((byte)0x10, "D#0");
			put((byte)0x11, "E0");
			put((byte)0x12, "F0");
			put((byte)0x13, "F#0");
			put((byte)0x14, "G0");
			put((byte)0x15, "G#0");
			put((byte)0x16, "A#0");
			put((byte)0x17, "B0");
			put((byte)0x18, "C1");
			put((byte)0x19, "C#1");
			put((byte)0x1A, "D1");
			put((byte)0x1B, "D#1");
			put((byte)0x1C, "E1");
			put((byte)0x1D, "F1");
			put((byte)0x1E, "F#1");
			put((byte)0x1F, "G1");
			put((byte)0x20, "G#1");
			put((byte)0x21, "A1");
			put((byte)0x22, "A#1");
			put((byte)0x23, "B1");
			put((byte)0x24, "C2");
			put((byte)0x25, "C#2");
			put((byte)0x26, "D2");
			put((byte)0x27, "D#2");
			put((byte)0x28, "E2");
			put((byte)0x29, "F2");
			put((byte)0x2A, "F#2");
			put((byte)0x2B, "G2");
			put((byte)0x2C, "G#2");
			put((byte)0x2D, "A2");
			put((byte)0x2E, "A#2");
			put((byte)0x2F, "B2");
			put((byte)0x30, "C3");
			put((byte)0x31, "C#3");
			put((byte)0x32, "D3");
			put((byte)0x33, "D#3");
			put((byte)0x34, "E3");
			put((byte)0x35, "F3");
			put((byte)0x36, "F#3");
			put((byte)0x37, "G3");
			put((byte)0x38, "G#3");
			put((byte)0x39, "A3");
			put((byte)0x3A, "A#3");
			put((byte)0x3B, "B3");
			put((byte)0x3C, "C4");
			put((byte)0x3D, "C#4");
			put((byte)0x3E, "D4");
			put((byte)0x3F, "D#4");
			put((byte)0x40, "E4");
			put((byte)0x41, "F4");
			put((byte)0x42, "F#4");
			put((byte)0x43, "G4");
			put((byte)0x44, "G#4");
			put((byte)0x45, "A4");
			put((byte)0x46, "A#4");
			put((byte)0x47, "B4");
			put((byte)0x48, "C5");
			put((byte)0x49, "C#5");
			put((byte)0x4A, "D5");
			put((byte)0x4B, "D#5");
			put((byte)0x4C, "E5");
			put((byte)0x4D, "F5");
			put((byte)0x4E, "F#5");
			put((byte)0x4F, "G5");
			put((byte)0x50, "G#5");
			put((byte)0x51, "A5");
			put((byte)0x52, "A#5");
			put((byte)0x53, "B5");
			put((byte)0x54, "C6");
			put((byte)0x55, "C#6");
			put((byte)0x56, "D6");
			put((byte)0x57, "D#6");
			put((byte)0x58, "E6");
			put((byte)0x59, "F6");
			put((byte)0x5A, "F#6");
			put((byte)0x5B, "G6");
			put((byte)0x5C, "G#6");
			put((byte)0x5D, "A6");
			put((byte)0x5E, "A#6");
			put((byte)0x5F, "B6");
			put((byte)0x60, "C7");
			put((byte)0x61, "C#7");
			put((byte)0x62, "D7");
			put((byte)0x63, "D#7");
			put((byte)0x64, "E7");
			put((byte)0x65, "F7");
			put((byte)0x66, "F#7");
			put((byte)0x67, "G7");
			put((byte)0x68, "G#7");
			put((byte)0x69, "A7");
			put((byte)0x6A, "A#7");
			put((byte)0x6B, "B7");
			put((byte)0x6C, "C8");
			put((byte)0x6D, "C#8");
			put((byte)0x6E, "D8");
			put((byte)0x6F, "D#8");
			put((byte)0x70, "E8");
			put((byte)0x71, "F8");
			put((byte)0x72, "F#8");
			put((byte)0x73, "G8");
			put((byte)0x74, "G#8");
			put((byte)0x75, "A8");
			put((byte)0x76, "A#8");
			put((byte)0x77, "B8");
			put((byte)0x78, "C9");
			put((byte)0x79, "C#9");
			put((byte)0x7A, "D9");
			put((byte)0x7B, "D#9");
			put((byte)0x7C, "E9");
			put((byte)0x7D, "F9");
			put((byte)0x7E, "F#9");
			put((byte)0x7F, "G9");
		}
	};

	static final Map<Byte, String> INSTRUMENT_MAP = new HashMap<>(){
        {
            put((byte)0x00, "Acoustic Grand Piano");
            put((byte)0x01, "Bright Acoustic Piano");
            put((byte)0x02, "Electric Grand Piano");
            put((byte)0x03, "Honky-Tonk Piano");
            put((byte)0x04, "Electric Piano 1");
            put((byte)0x05, "Electric Piano 2");
            put((byte)0x06, "Harpsichord");
            put((byte)0x07, "Clavi");
            put((byte)0x08, "Celesta");
            put((byte)0x09, "Glockenspiel");
            put((byte)0x0A, "Music Box");
            put((byte)0x0B, "Vibraphone");
            put((byte)0x0C, "Marimba");
            put((byte)0x0D, "Xylophone");
            put((byte)0x0E, "Tubular Bells");
            put((byte)0x0F, "Dulcimer");
            put((byte)0x10, "Drawbar Organ");
            put((byte)0x11, "Percussive Organ");
            put((byte)0x12, "Rock Organ");
            put((byte)0x13, "Church Organ");
            put((byte)0x14, "Reed Organ");
            put((byte)0x15, "Accordion");
            put((byte)0x16, "Harmonica");
            put((byte)0x17, "Tango Accordion");
            put((byte)0x18, "Acoustic Guitar (nylon)");
            put((byte)0x19, "Acoustic Guitar (steel)");
            put((byte)0x1A, "Electric Guitar (jazz)");
            put((byte)0x1B, "Electric Guitar (clean)");
            put((byte)0x1C, "Electric Guitar (muted)");
            put((byte)0x1D, "Overdriven Guitar");
            put((byte)0x1E, "Distortion Guitar");
            put((byte)0x1F, "Guitar Harmonics");
            put((byte)0x20, "Acoustic Bass");
            put((byte)0x21, "Electric Bass (finger)");
            put((byte)0x22, "Electric Bass (pick)");
            put((byte)0x23, "Fretless Bass");
            put((byte)0x24, "Slap Bass 1");
            put((byte)0x25, "Slap Bass 2");
            put((byte)0x26, "Synth Bass 1");
            put((byte)0x27, "Synth Bass 2");
            put((byte)0x28, "Violin");
            put((byte)0x29, "Viola");
            put((byte)0x2A, "Cello");
            put((byte)0x2B, "Contrabass");
            put((byte)0x2C, "Tremolo Strings");
            put((byte)0x2D, "Pizzicato Strings");
            put((byte)0x2E, "Orchestral Harp");
            put((byte)0x2F, "Timpani");
            put((byte)0x30, "String Ensemble 1");
            put((byte)0x31, "String Ensemble 2");
            put((byte)0x32, "SynthStrings 1");
            put((byte)0x33, "SynthStrings 2");
            put((byte)0x34, "Choir Aahs");
            put((byte)0x35, "Voice Oohs");
            put((byte)0x36, "Synth Voice");
            put((byte)0x37, "Orchestra Hit");
            put((byte)0x38, "Trumpet");
            put((byte)0x39, "Trombone");
            put((byte)0x3A, "Tuba");
            put((byte)0x3B, "Muted Trumpet");
            put((byte)0x3C, "French Horn");
            put((byte)0x3D, "Brass Section");
            put((byte)0x3E, "SynthBrass 1");
            put((byte)0x3F, "SynthBrass 2");
            put((byte)0x40, "Soprano Sax");
            put((byte)0x41, "Alto Sax");
            put((byte)0x42, "Tenor Sax");
            put((byte)0x43, "Baritone Sax");
            put((byte)0x44, "Oboe");
            put((byte)0x45, "English Horn");
            put((byte)0x46, "Bassoon");
            put((byte)0x47, "Clarinet");
            put((byte)0x48, "Piccolo");
            put((byte)0x49, "Flute");
            put((byte)0x4A, "Recorder");
            put((byte)0x4B, "Pan Flute");
            put((byte)0x4C, "Blown Bottle");
            put((byte)0x4D, "Shakuhachi");
            put((byte)0x4E, "Whistle");
            put((byte)0x4F, "Ocarina");
            put((byte)0x50, "Lead 1 (Square)");
            put((byte)0x51, "Lead 2 (Sawtooth)");
            put((byte)0x52, "Lead 3 (Calliope)");
            put((byte)0x53, "Lead 4 (Chiff)");
            put((byte)0x54, "Lead 5 (Charang)");
            put((byte)0x55, "Lead 6 (Voice)");
            put((byte)0x56, "Lead 7 (Fifths)");
            put((byte)0x57, "Lead 8 (Bass + Lead)");
            put((byte)0x58, "Pad 1 (New Age)");
            put((byte)0x59, "Pad 2 (Warm)");
            put((byte)0x5A, "Pad 3 (Polysynth)");
            put((byte)0x5B, "Pad 4 (Choir)");
            put((byte)0x5C, "Pad 5 (Bowed)");
            put((byte)0x5D, "Pad 6 (Metallic)");
            put((byte)0x5E, "Pad 7 (Halo)");
            put((byte)0x5F, "Pad 8 (Sweep)");
            put((byte)0x60, "FX 1 (Rain)");
            put((byte)0x61, "FX 2 (Soundtrack)");
            put((byte)0x62, "FX 3 (Crystal)");
            put((byte)0x63, "FX 4 (Atmosphere)");
            put((byte)0x64, "FX 5 (Brightness)");
            put((byte)0x65, "FX 6 (Goblins)");
            put((byte)0x66, "FX 7 (Echoes)");
            put((byte)0x67, "FX 8 (Sci-Fi)");
            put((byte)0x68, "Sitar");
            put((byte)0x69, "Banjo");
            put((byte)0x6A, "Shamisen");
            put((byte)0x6B, "Koto");
            put((byte)0x6C, "Kalimba");
            put((byte)0x6D, "Bag Pipe");
            put((byte)0x6E, "Fiddle");
            put((byte)0x6F, "Shanai");
            put((byte)0x70, "Tinkle Bell");
            put((byte)0x71, "Agogo");
            put((byte)0x72, "Steel Drums");
            put((byte)0x73, "Woodblock");
            put((byte)0x74, "Taiko Drum");
            put((byte)0x75, "Melodic Tom");
            put((byte)0x76, "Synth Drum");
            put((byte)0x77, "Reverse Cymbal");
            put((byte)0x78, "Guitar Fret Noise");
            put((byte)0x79, "Breath Noise");
            put((byte)0x7A, "Seashore");
            put((byte)0x7B, "Bird Tweet");
            put((byte)0x7C, "Telephone Ring");
            put((byte)0x7D, "Helicopter");
            put((byte)0x7E, "Applause");
            put((byte)0x7F, "Gunshot");
        }
    };
}
