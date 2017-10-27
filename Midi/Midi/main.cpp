#include "Midi.h"

using namespace std;
using namespace mid;

int main(int argc, char** argv) {
	//uncomment the following to test variable length values
	//uint low = 0x0000007F;
	//uint low_mid = 0x00003FFF;
	//uint high_mid = 0x001FFFFF;
	//uint high = 0x0FFFFFFF;
	//vlv lowvlv = low;
	//vlv lowmidvlv = low_mid;
	//vlv highmidvlv = high_mid;
	//vlv highvlv = high;
	//lowvlv--;
	//lowvlv++;
	//lowvlv++;
	//lowvlv--;
	//uint i = lowvlv % 4U;
	
	//uncomment the following to test toString&fromString stuffs
	//vector<Chunk*> chunks;
	//chunks.push_back(new HeaderChunk());
	//Midi midi{ chunks };
	//char* bitString = midi.toBitString();
	//Midi midi2;
	//midi2.fromBitString(bitString, midi.getLength());

	//uncomment the following to test file stuffs
	Midi midi;
	File::read("jauntyambience.mid", midi);
	File::write("jauntyambience1.mid", midi);
	system("pause");
	return EXIT_SUCCESS;
}