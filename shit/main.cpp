#include <iostream>
#include <string>
#include <sstream>

#include "Memory.h"
#include "CPU.h"
#include "ReadyQueue.h"
#include "IODevice.h"

using namespace std;
bool validateNumber(string x) {
	for (int i = 0; i < x.length(); i++) {
		if (isalpha(x[i]) || ispunct(x[i]))
			return false;
 	}
	return true;
}

int getInput(string prompt) {
	string temp;
	unsigned int s = 0;
	while(s == 0) {
		cout << prompt << " ";
		getline(cin, temp);
		if (validateNumber(temp))
			s = stoll(temp);
		else
			cout << "Invalid input. Please try again." << endl;
	}
	return s;
}

int main() {
	cout << "Starting OS Simulation.." << endl;
	Memory ram(getInput("How many bytes are in your computer?"));
	IODevice harddisks(getInput("How many hard disks would you like?"), "Hard Disk");
	IODevice printers(getInput("How many printers would you like?"), "Printer");
	
	while (true) {

		string input = "";
		string command = "";
		cout << "Input command : ";
		getline(cin, input);

		stringstream ss(input);
		ss >> command;
		
		if (command.compare("A") == 0) {
			string tok1 = "x";
			string tok2 = "x";
			int priority;
			unsigned int size;
			ss >> tok1 >> tok2;
			if (validateNumber(tok1) && validateNumber(tok2)) {
				priority = stoi(tok1);
				size = stoll(tok2);
			} else {
				cout << "Invalid input. Please try again." << endl;
				continue;
			}
			ram.insert(priority, size);
		}
		else if (command.compare("t") == 0) {
			CPU::finishProcess();
		}
		else if (command.compare("d") == 0) {
			string tok1 = "x";
			unsigned int index;
			ss >> tok1;
			if (validateNumber(tok1))
				index = stoi(tok1);
			else {
				cout << "Invalid input. Please try again." << endl;
				continue;
			}
			CPU::sendToDevice(index, harddisks);
		}
		else if (command.compare("p") == 0) {
			string tok1 = "x";
			unsigned int index;
			ss >> tok1;
			if (validateNumber(tok1))
				index = stoi(tok1);
			else {
				cout << "Invalid input. Please try again." << endl;
				continue;
			}
			CPU::sendToDevice(index, printers);
		}
		else if (command.compare("D") == 0) {
			string tok1 = "x";
			unsigned int index;
			ss >> tok1;
			if (validateNumber(tok1))
				index = stoi(tok1);
			else {
				cout << "Invalid input. Please try again." << endl;
				continue;
			}
			harddisks.releaseProcess(index);
		}
		else if (command.compare("P") == 0) {
			string tok1 = "x";
			unsigned int index;
			ss >> tok1;
			if (validateNumber(tok1))
				index = stoi(tok1);
			else {
				cout << "Invalid input. Please try again." << endl;
				continue;
			}
			printers.releaseProcess(index);
		}
		else if (command.compare("S") == 0) {
			string type;
			ss >> type;
			if (type.compare("m") == 0) {
				ram.screenshot();
			}
			else if (type.compare("i") == 0) {
				harddisks.printQ();
				cout << endl;
				printers.printQ();
			}
			else if (type.compare("r") == 0) {
				CPU::printActive();
				ReadyQueue::printQs();
			}
		}
		cout << endl;
	}
}