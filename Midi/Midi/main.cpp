#include "Midi.h"

using namespace std;
using namespace mid;

int main(int argc, char** argv) {
	uint low = 0x0000007F;
	uint low_mid = 0x00003FFF;
	uint high_mid = 0x001FFFFF;
	uint high = 0x0FFFFFFF;
	vlv lowvlv = low;
	vlv lowmidvlv = low_mid;
	vlv highmidvlv = high_mid;
	vlv highvlv = high;
	lowvlv--;
	lowvlv++;
	lowvlv++;
	lowvlv--;
	uint i = lowvlv % 4U;
	system("pause");
	return 0;
}