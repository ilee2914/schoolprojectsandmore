#include "Memory.h"
#include "ReadyQueue.h"

#include <iostream>

using namespace std;

unsigned int Memory::maxVal = 0;
Process* Memory::root = nullptr;

Memory::Memory() {
	maxVal = 0;
	numOfProcesses = 1;
	root = nullptr;
}

Memory::Memory(int m) {
	maxVal = m;
	numOfProcesses = 1;
	root = new Process(-1, 0, 0, m - 1, m - 0, nullptr, nullptr, true);
}

void Memory::insert(int priority, unsigned int size) {
	Process* currptr = root;
	bool foundSpot = false;
	if (priority < 0) {
		priority = 0;
	}
	else if (priority > 4) {
		priority = 4;
	}
	while (currptr->next) {
		if (currptr->empty) {
			if (currptr->size >= size) {
				int nStart = currptr->start;
				int nEnd = nStart + size - 1;

				Process * temp = new Process(numOfProcesses, priority, nStart, nEnd, size, currptr->prev, currptr, false);
				if (currptr->size > size) {
					currptr->start = nEnd + 1;
					currptr->size -= size;
					currptr->prev = temp;
				}
				else {
					temp->next = currptr->next;
				}
				if (nStart == 0) {
					root = temp;
				}
				else {
					temp->prev->next = temp;
				}
				foundSpot = true;
				ReadyQueue::addProcess(temp);
				break;
			}
		}
		currptr = currptr->next;
	}

	if (currptr->empty && !foundSpot) {
		if (currptr->size >= size) {
			int nStart = currptr->start;
			int nEnd = nStart + size - 1;
			Process * temp = new Process(numOfProcesses, priority, nStart, nEnd, size, currptr->prev, currptr, false);
			if (currptr->size > size) {
				currptr->start = nEnd + 1;
				currptr->size -= size;
				currptr->prev = temp;
			}
			else {
				temp->next = currptr->next;
			}
			if (nStart == 0) {
				root = temp;
			}
			else {
				temp->prev->next = temp;
			}
			foundSpot = true;
			ReadyQueue::addProcess(temp);
		}
	}
	if (!foundSpot) {
		cout << "No spot could be found for a process with size : " << size << " bytes." << endl;
	}
	else {
		cout << "Process created!" << endl;
		numOfProcesses++;
	}
}

void Memory::screenshot() {
	cout << "RAM screenshot" << endl;
	Process * currptr = root;
	while (currptr->next) {
		cout << "PID : " << currptr->processNumber << " Space : " << currptr->start << " - " << currptr->end << " Hole : " << currptr->empty << endl;
		currptr = currptr->next;
	}
	cout << "PID : " << currptr->processNumber << " Space : " << currptr->start << " - " << currptr->end << " Hole : " << currptr->empty << endl;
}

void Memory::deleteProcess(Process* toDel) {
	Process* currptr = toDel;
	currptr->processNumber = -1;
	currptr->priority = -1;
	currptr->empty = true;
	if (currptr->prev != nullptr && currptr->prev->empty) {
		currptr->prev->end = currptr->end;
		currptr->prev->size += currptr->size;
		currptr->prev->next = currptr->next;
		if (currptr->next != nullptr)
			currptr->next->prev = currptr->prev;
		currptr = currptr->prev;
		delete toDel;
	}
	if (currptr->next != nullptr && currptr->next->empty) {
		currptr->next->start = currptr->start;
		currptr->next->size += currptr->size;
		currptr->next->prev = currptr->prev;
		if (currptr->prev != nullptr)
			currptr->prev->next = currptr->next;
		if (currptr->next->size >= maxVal)
			root = currptr->next;
		delete currptr;
	}
}