#include "HardDisk.h"
#include "Schedule.h"

#include <iostream>

void HardDisk::addProcess(Process * p, string fn) {
	HardDiskEntry * temp = new HardDiskEntry(p, fn);
	hd.push_back(temp);
}

void HardDisk::returnProcess(Schedule * q) {
	Process * temp = hd.front()->p;
	hd.pop_front();
	q->addToQueues(temp);
}

void HardDisk::print() {
	for (HardDiskEntry * e : hd) {
		std::cout << e->p->pid << " ";
	}
}
HardDisk::~HardDisk() {
}
