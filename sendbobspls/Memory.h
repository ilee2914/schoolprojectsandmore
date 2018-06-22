#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

class CPU;
class Schedule;
struct Process;

struct PageEntry {
	PageEntry() {
		page = -1;
		pid = -1;
		timeSlice = -1;
	}
	PageEntry(int p, int id, int ts) {
		page = p;
		pid = id;
		timeSlice = ts;
	}

	int page;
	int pid;
	int timeSlice = -1;
};

class Memory {
public:
	Memory(unsigned int size, unsigned int pageSize);

	void createNewProcess(CPU * c, Schedule *q);
	int leastUsedFrame();
	void terminate(int pid);
	void accessAddress(Process *p, int num);
	void print();

	~Memory();

private:
	unsigned int processCounter;
	unsigned int size;
	unsigned int pageSize;
	unsigned int numOfFrames;

	std::vector<PageEntry*> pageTable;
	int timeSlice;
};

#endif