#include "CPU.h"
#include "Schedule.h"
#include "Memory.h"
#include "HardDisk.h"
#include <iostream>

CPU::CPU() {
	currentProcess = nullptr;
}
void CPU::loadProcess(Process *temp, Schedule *q) {
	sendToQueues(q);
	currentProcess = temp;
}

void CPU::sendToQueues(Schedule *q) {
	q->addToQueues(currentProcess);
	currentProcess = nullptr;
}

void CPU::terminate(Memory *mem) {
	mem->terminate(currentProcess->pid);
	//delete currentProcess;
	//currentProcess = nullptr;
}

void CPU::fetchProcess(Schedule * q) {
	if (!currentProcess) {
		cout << "No current process" << endl;
		return;
	}
	currentProcess = q->getNextProcess();
}

void CPU::passQuantum() {
	if (currentProcess)
		currentProcess->quantum++;
}

void CPU::sendToDisk(Schedule *q, HardDisk *hd, std::string filename) {
	if (!currentProcess) {
		cout << "No current process" << endl;
		return;
	}
	hd->addProcess(currentProcess, filename);
	currentProcess = q->getNextProcess();
}

void CPU::accessAddress(int num, Memory* mem) {
	if (!currentProcess) {
		cout << "No current process" << endl;
		return;
	}
	mem->accessAddress(currentProcess, num);
}

Process* CPU::getCurrent() {
	return currentProcess;
}
void CPU::print() {
	if (currentProcess)
		cout << "Current CPU Process:" << currentProcess->pid << endl;
	else
		cout << "CPU is free." << endl;
}