#include "Memory.h"
#include "CPU.h"
#include "Schedule.h"
#include <iostream>
#include <limits>

Memory::Memory(unsigned int s, unsigned int ps) {
	size = s;
	pageSize = ps;
	numOfFrames = size / ps;
	pageTable.resize(numOfFrames);
	for (int i = 0; i < pageTable.size(); i++) {
		pageTable[i] = new PageEntry(-1, -1, -1);
	}
	processCounter = 0;
	timeSlice = 0;
}

void Memory::createNewProcess(CPU * c, Schedule * q) {
	processCounter++;
	timeSlice++;
	Process * temp =  new Process(processCounter, 0, timeSlice);
	pageTable[leastUsedFrame()] = new PageEntry(0, processCounter, timeSlice);
	if (c->getCurrent() == nullptr) {
		c->loadProcess(temp, q);
	} else {
		q->addToQueues(temp);
	}
}

int Memory::leastUsedFrame() {
	int min = std::numeric_limits<int>::max();
	int index = -1;

	for (unsigned int i = 0; i < pageTable.size(); i++) {
		if (pageTable[i] == NULL) {
			return i;
		} else if (pageTable[i]->timeSlice < min) {
			min = pageTable[i]->timeSlice;
			index = i;
		}
	}
	return index;
}

// Clear the page table of the pid.
// Sets everythign to -1.
void Memory::terminate(int pid) {
	for (unsigned int i = 0; i < pageTable.size(); i++) {
		if (pageTable[i] != NULL && pageTable[i]->pid == pid) {
			pageTable[i]->page = -1;
			pageTable[i]->pid = -1;
			pageTable[i]->timeSlice = -1;
		}
	}
}

void Memory::accessAddress(Process *p, int num) {
	if (p == nullptr) return;

	int page = num / pageSize;
	int pid = p->pid;
	unsigned int i;
	int index = -1;
	timeSlice++;

	for (i = 0; i < pageTable.size(); i++) {
		if ((pageTable[i] == NULL || pageTable[i]->timeSlice == -1) && index != -1) {
			index = i;
		}
		if (pageTable[i] != NULL && pid == pageTable[i]->pid && page == pageTable[i]->page) {
			pageTable[i]->timeSlice = timeSlice;
			return;
		}
	}

	if (index != -1) {
		pageTable[index] = new PageEntry(page, pid, timeSlice);
	} else {
		int luf = leastUsedFrame();
		pageTable[luf]->page = page;
		pageTable[luf]->pid = pid;
		pageTable[luf]->timeSlice = timeSlice;
	}
	
}

void Memory::print() {
	cout << "F P PID T" << endl;
	for (unsigned int i = 0; i < pageTable.size(); i++) {
		if (pageTable[i] == NULL || pageTable[i]->timeSlice == -1) 
			continue;
		cout << i << " " << pageTable[i]->page << "   " << pageTable[i]->pid << " " << pageTable[i]->timeSlice << endl;
	}
}

Memory::~Memory() {

}
