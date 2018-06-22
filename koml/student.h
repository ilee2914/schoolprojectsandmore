#include <iostream>
#include <string>

using namespace std;

class Student {
	public:
		Student() {
			name = "null";
			grade = 0;
			next = nullptr;
		}
		Student(string n, int g) {
			name = n;
			grade = g;
			next = nullptr;
		}
		
		Student* getNext() {return next;}
		string getName() {return name;}
		int getGrade() {return grade;}
		
		void setNext(Student* n) {next = n;}
		void setName(string n) {name = n;}
		void setGrade(int g) {grade = g;}
		
	private:
		string name;
		int grade;
		Student* next;
};