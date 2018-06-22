#ifndef HARDDISK_H
#define HARDDISK_H

#include <string>
#include <deque>

struct Process;
class Schedule;

using namespace std;

struct HardDiskEntry {
	HardDiskEntry(Process *pr, std::string fn) {
		p = pr;
		file_name = fn;
	}
	Process * p;
	std::string file_name;
};

class HardDisk {
public:
	HardDisk() = default;

	void addProcess(Process *p, std::string fn);
	void returnProcess(Schedule *q);
	void print();

	~HardDisk();

private:
	deque<HardDiskEntry*> hd;
};


#endif