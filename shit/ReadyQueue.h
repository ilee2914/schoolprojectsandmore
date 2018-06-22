#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include <deque>
#include <list>
#include <queue>
#include "Memory.h"
#include "cont.h"

using namespace std;
//Priority Scheduling Ready Queue
// 0 lowest priority
// 4 highest priority


class ReadyQueue {
public:
	static void addProcess(Process * mn);
	static void printQs();
	static Process* chooseNewProcess();
private:
	static cont < queue<Process*, list<Process*> > >* rQ;
};

#endif