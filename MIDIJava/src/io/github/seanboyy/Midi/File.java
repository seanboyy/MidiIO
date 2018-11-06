package io.github.seanboyy.Midi;

import java.io.DataInputStream;
import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.DataOutputStream;
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;

class File{
	static Midi read(String filename) throws IOException{
		java.io.File file = new java.io.File(filename);
		DataInputStream dis = new DataInputStream(new BufferedInputStream(new FileInputStream(file)));
		byte[] fileString = new byte[(int)file.length()];
		int count = dis.read(fileString);
		assert(count == (int)file.length());
		Midi midi = new Midi();
		midi.fromBitString(fileString, (int)file.length());
		dis.close();
		return midi;
	}
	
	static void write(String filename, Midi midi) throws IOException{
		java.io.File file = new java.io.File(filename);
		DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(file)));
		dos.write(midi.toBitString(), 0, midi.getLength());
		dos.flush();
		dos.close();
	}
}
