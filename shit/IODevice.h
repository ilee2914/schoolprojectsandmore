#ifndef IODEVICE_H
#define IODEVICE_H

#include <iostream>
#include <queue>
#include <deque>
#include <list>
#include "Memory.h"
#include "cont.h"

using namespace std;
class IODevice {
public:
	IODevice();
	IODevice(int s, string t);

	void addProcess(int i, Process* toAdd);
	void releaseProcess(int i);
	void printQ();
	string getType();
	int getSize();
private:
	cont<queue<Process*, list<Process*> >>* container;
	int size;
	string type;
};

#endif