#include <iostream>
#include <string>
#include "student.h"

using namespace std;

class StudentLinkedList {
	public:
		StudentLinkedList() {
			root = nullptr;
		}
		
		// Adds student to the beginning of linked list;
		void add(string name, int grade) {
			Student* stud =  new Student(name, grade);
			if (root == nullptr) {
				root = stud;
				cout << name << " added";
				return;
			}
			stud->setNext(root);
			root = stud;
			cout << name << " added";
		}
		void remove(string name) {
			Student* prevPtr = root;
			Student* nextPtr = root->getNext();
			if (prevPtr->getName() == name) {
				root = nextPtr;
				delete prevPtr;
				cout << "Done." << endl;
				return;
			}
			
			while (nextPtr != nullptr) {
				if (nextPtr->getName() == name) {
					prevPtr->setNext(nextPtr->getNext());
					delete nextPtr;
					cout << "Done." << endl;
					return;
				}
				prevPtr = nextPtr;
				nextPtr = nextPtr->getNext();
			}
			
			cout << "Done." << endl;
		}
		void update(string name, int grade) {
			Student* currPtr = root;
			while (currPtr != nullptr) {
				if (currPtr->getName() == name) {
					currPtr->setGrade(grade);
					cout << "Updated " << name;
					return;
				}
				currPtr = currPtr->getNext();
			}
			cout << "Student not found" << endl;
		}
		
		void search(string name) {
			Student* currPtr = root;
			while (currPtr != nullptr) {
				if (currPtr->getName() == name) {
					cout << "Student name : " << currPtr->getName() << "\n"
						 << "Grade : " << currPtr->getGrade() << endl;
					return;
				}
				currPtr = currPtr->getNext();
			}
			cout << "Student not found" << endl;
		}
	private:
		Student* root;
};