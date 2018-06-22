#include "CPU.h"
#include "ReadyQueue.h"
#include <iostream>

Process* CPU::currentProcess = nullptr;

void CPU::contextSwitch(Process* otherProcess) {
	if (currentProcess != nullptr) {
		ReadyQueue::addProcess(currentProcess);
	}
	currentProcess = otherProcess;
}

int CPU::currentPriority() {
	if (currentProcess != nullptr)
		return currentProcess->priority;
	return -1;
}

void CPU::printActive() {
	if (currentProcess == nullptr)
		cout << "PID of active process : -1" << endl;
	else
		cout << "PID of active process : " << currentProcess->processNumber << endl;
}

void CPU::finishProcess() {
	if (currentProcess == nullptr) {
		cout << "No process active" << endl;
		return;
	}
	cout << "Finishing process " << currentProcess->processNumber << endl;
	Memory::deleteProcess(currentProcess);
	currentProcess = ReadyQueue::chooseNewProcess();
	if (currentProcess == nullptr) {
		cout << "No process to replace with." << endl;
		return;
	}
	else {
		cout << "Replacing with " << currentProcess->processNumber << endl;
	}
	
}

void CPU::sendToDevice(int i, IODevice& dev) {
	if (i < 0) {
		i = 0;
	}
	else if (i > dev.getSize()-1) {
		i = dev.getSize() - 1;
	}
	dev.addProcess(i, currentProcess);
	string t = dev.getType();
	if (currentProcess != nullptr) {
		cout << "Sent process " << currentProcess->processNumber << " to " << t.c_str() << " " << i << endl;
		currentProcess = ReadyQueue::chooseNewProcess();

		if (currentProcess == nullptr) {
			cout << "No process to replace with." << endl;
			return;
		}
		else {
			cout << "Replacing with " << currentProcess->processNumber << endl;
		}
	}
	else {
		cout << "No active process" << endl;
	}
}