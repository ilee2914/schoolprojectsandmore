#include <fstream>
#include <stdlib.h>
using namespace std;

//jle = 0x7e
//address = 0x58f
//Input hex values
int main(int argc, char *argv[]) {
	if (argc < 4) {
		cout << "Usage : changePacket filename address packet IN HEX";
		return 0;
	}
	char packet[1] = {strtol(argv[3], new char*, 16)};
	fstream file(argv[1], ios::in | ios::out | ios::binary);

	file.seekg(strtol(argv[2], new char*, 16), ios::beg);
	file.write((char*)packet, sizeof(packet));
}