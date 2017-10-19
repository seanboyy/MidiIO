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
	
	//uncomment the following to test toString stuffs
	vector<Chunk*> chunks;
	chunks.push_back(new HeaderChunk());
	Midi midi{ chunks };
	char* bitString = midi.toBitString();
	system("pause");
	return EXIT_SUCCESS;
}