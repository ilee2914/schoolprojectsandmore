#ifndef MEMORY_H
#define MEMORY_H

struct Process {
	Process(int n, int pr, unsigned int st, unsigned int e, unsigned int si, Process * p, Process * ne, bool em) {
		processNumber = n;
		priority = pr;
		start = st;
		end = e;
		size = si;
		prev = p;
		next = ne;
		empty = em;
	}

	int processNumber;
	int priority;
	unsigned int start;
	unsigned int end;
	unsigned int size;
	Process* prev;
	Process* next;
	bool empty;
};

class Memory {
public:
	Memory();
	Memory(int m);

	void insert(int priority, unsigned int size);
	static void deleteProcess(Process*);

	void screenshot();
private:
	static unsigned int maxVal;
	int numOfProcesses;
	static Process* root;
};

#endif