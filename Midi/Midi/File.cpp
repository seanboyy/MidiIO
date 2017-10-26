#include "Midi.h"

namespace mid {
	bool File::read(const char* filename, Midi& midi) {
		std::ifstream fin(filename, std::ios::binary);
		if (fin.fail()) {
			return false;
		}
		char temp;
		uint filesize = 0;
		while (!fin.eof()) {
			temp = fin.get();
			filesize++;
		}
		filesize--;
		fin.close();
		fin.open(filename, std::ios::binary);
		char* fileData = new char[filesize];
		fin.read(fileData, filesize);
		midi.fromBitString(fileData, filesize);
		return true;
	}

	bool File::write(const char* filename, Midi midi) {
		std::ofstream fout(filename, std::ios::binary);
		if (fout.fail()) {
			return false;
		}
		uint filesize = midi.getLength();
		fout.write(midi.toBitString(), filesize);
		fout.close();
		return true;
	}
}