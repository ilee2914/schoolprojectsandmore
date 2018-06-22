#ifndef CPU_H
#define CPU_H

#include <string>

class HardDisk;
struct Process;
class Schedule;
class Memory;

class CPU {
public:
	CPU();

	void sendToDisk(Schedule *q, HardDisk *hd, std::string filename);
	void sendToQueues(Schedule * q);
	void terminate(Memory * mem);
	void loadProcess(Process *p, Schedule * q);
	void fetchProcess(Schedule * q);
	void passQuantum();
	void accessAddress(int num, Memory * mem);

	void print();
	Process* getCurrent();

	~CPU() = default;

private:
	Process *currentProcess;

};

#endif