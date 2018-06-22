#include "IODevice.h"
#include "ReadyQueue.h"
#include <string>

IODevice::IODevice() {
	
}

IODevice::IODevice(int s, string t) {
	size = s;
	type = t;
	container = new cont<queue<Process*, list<Process*> >>[s];
}

void IODevice::addProcess(int i, Process* toAdd) {
	container[i].push(toAdd);
}

void  IODevice::releaseProcess(int i) {
	if (!container[i].empty()) {
		Process* temp = container[i].front();
		container[i].pop();
		cout << "Sending " << temp->processNumber << " back into RQ" << endl;
		ReadyQueue::addProcess(temp);
	}
	else {
		cout << "No active process. " << endl;
	}
}

void IODevice::printQ() {
	cout << endl;
	for (int i = 0; i < size; i++) {
		cout << "Printing " << type << " " << i << " queue : ";
		bool active = true;
		for (Process* x : container[i].getContainer()) {
			if (active)
				cout << "Active : ";
			cout << x->processNumber << " ";
			if (active)
				cout << "Waiting : ";
			active = false;
		}
		cout << endl;
	}
}

string IODevice::getType() {
	return type;
}

int IODevice::getSize() {
	return size;
}