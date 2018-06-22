#ifndef CPU_H
#define CPU_H

#include "Memory.h"
#include "IODevice.h"

class CPU {
public:
	static void contextSwitch(Process* otherProcess);
	static int currentPriority();
	static void printActive();
	static void finishProcess();
	static void sendToDevice(int i, IODevice& dev);
private:
	static Process* currentProcess;
};

#endif