#include "Schedule.h"
#include <iostream>


Schedule::Schedule() {
	q.resize(3);
}


Process * Schedule::getNextProcess() {
	if (!q[0].empty()) {
		Process * temp = q[0].front();
		q[0].pop_front();
		return temp;
	} else if (!q[1].empty()) {
		Process * temp = q[1].front();
		q[1].pop_front();
		return temp;
	}  else if (!q[2].empty()) {
		Process * temp = q[2].front();
		q[2].pop_front();
		return temp;
	}
	std::cout << "No waiting process";
	return nullptr;
}

void Schedule::addToQueues(Process * p) {
	if (p != nullptr) {
		int quantum = p->quantum;
		switch (quantum) {
		case 0:
			q[0].push_back(p);
			break;
		case 1:
		case 2:
			q[1].push_back(p);
			break;
		default:
			q[2].push_back(p);
			break;
		}
	}
}

void Schedule::print() {
	cout << "Level 0 : ";
	for (Process * p : q[0]) {
		cout << p->pid << " ";
	}
	cout << endl;

	cout << "Level 1 : ";
	for (Process * p : q[1]) {
		cout << p->pid << " ";
	}
	cout << endl;

	cout << "Level 2 : ";
	for (Process * p : q[2]) {
		cout << p->pid << " ";
	}
	cout << endl;
}

Schedule::~Schedule() {
}
