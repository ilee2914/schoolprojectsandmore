#include "ReadyQueue.h"
#include "CPU.h"
#include <iostream>

cont<queue<Process*, list<Process*>>>* ReadyQueue::rQ = new cont<queue<Process*, list<Process*>>>[5];

void ReadyQueue::addProcess(Process * p) {
	int priority = p->priority;
	if (priority > CPU::currentPriority()) {
		CPU::contextSwitch(p);
	}
	else {
		rQ[priority].push(p);
	}
}

void ReadyQueue::printQs() {
	for (int i = 0; i < 5; i++) {
		cout << "Printing " << i << " level queue : ";
		for (Process* x : rQ[i].getContainer()) {
			cout << x->processNumber << " ";
		}
		cout << endl;
	}
}

Process* ReadyQueue::chooseNewProcess() {
	for (int i = 4; i >= 0; i--) {	
		list<Process*> x = rQ[i].getContainer();
		if (x.size() > 0) {
			Process* temp = rQ[i].front();
			rQ[i].pop();
			return temp;
		}
	}
	return nullptr;
}