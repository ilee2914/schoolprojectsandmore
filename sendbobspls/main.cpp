#include <iostream>
#include <string>
#include <sstream>

#include "Memory.h"
#include "CPU.h"
#include "HardDisk.h"
#include "Schedule.h"

using namespace std;

int main() {
	unsigned int memSize;
	int pgfrSize;
	int hardDisks;
	cout << "OS SIMULATOR" << endl;
	cout << "How much memory does your computer have?";
	cin >> memSize;
	cout << "What is the page/frame size? ";
	cin >> pgfrSize;
	cout << "What are the number of hard disks? ";
	cin >> hardDisks;
	cin.ignore();

	Memory * mem = new Memory(memSize, pgfrSize);
	vector<HardDisk*> hd;
	hd.resize(hardDisks);
	for (int i = 0; i < hardDisks; i++) {
		hd[i] = new HardDisk();
	}
	CPU *c = new CPU();
	Schedule *q = new Schedule();

	string input = "";
	string command = "";

	while (true) {
		cout << "Input command : " << endl;
		getline(cin, input);
		stringstream ss(input);
		ss >> command;

		if (command.compare("A") == 0) {
			mem->createNewProcess(c, q);
		} else if (command.compare("Q") == 0) {
			c->passQuantum();
		} else if (command.compare("t") == 0) {
			c->terminate(mem);
		} else if (command.compare("d") == 0) {
			string number;
			unsigned int n;
			string filename;
			ss >> number;
			n = stoi(number);
			ss >> filename;
			if (n > hd.size()) {
				cout << "Invalid hard disk." << endl;
			} else {
				c->sendToDisk(q, hd[n], filename);
			}
		} else if (command.compare("D") == 0) {
			string number;
			unsigned int n;
			ss >> number;
			n = stoi(number);
			if (n > hd.size()) {
				cout << "Invalid hard disk." << endl;
			} else {
				hd[n]->returnProcess(q);
			}
		} else if (command.compare("m") == 0) {
			string number;
			int n;
			ss >> number;
			n = stoi(number);
			c->accessAddress(n, mem);
		} else if (command.compare("S") == 0) {
			string flag;
			ss >> flag;
			if (flag.compare("r") == 0) {
				c->print();
				q->print();
			} else if (flag.compare("i") == 0) {
				for (int i = 0; i < hd.size(); i++) {
					cout << "Hard Drive " << i << ": ";
					hd[i]->print();
					cout << endl;
				}
			} else if (flag.compare("m") == 0) {
				mem->print();
			}
		} else {
			cout << "Invalid command : " << command << endl;
		}
		cout << endl;
	}
 	


}
