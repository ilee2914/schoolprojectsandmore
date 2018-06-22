#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <deque>
#include <vector>
using namespace std;

struct Process {
	Process(int p, int s, int q) {
		pid = p;
		state = s;
		quantum = 0;
	}
	int pid;
	int state;
	int quantum;
};


class Schedule {
public:
	Schedule();

	vector<deque<Process*>> getQueue() { return q; }
	Process * getNextProcess();
	void addToQueues(Process *p);
	void print();
	~Schedule();

private:
	vector<deque<Process*>> q;
};

#endif