#include <iostream>
#include "StudentLinkedList.h"

using namespace std;

int main() {
	StudentLinkedList myClass;
	while (true) {
		int option;
		cout << "What would you like to do?" << "\n"
			 << "1. Add a student" << "\n"
			 << "2. Remove a student" << "\n"
			 << "3. Update a grade" << "\n"
			 << "4. Search a student" << endl << endl;
		cin >> option;
		if (option == 1) {
			string name;
			int grade;
			cout << "What is the student's name? ";
			cin >> name;
			cout << "What is the student's grade? ";
			cin >> grade;
			myClass.add(name, grade);
		} else if (option == 2) {
			string name;
			cout << "What is the student's name? ";
			cin >> name;
			myClass.remove(name);
		} else if (option == 3) {
			string name;
			int grade;
			cout << "What is the student's name? ";
			cin >> name;
			cout << "What is the student's new grade? ";
			cin >> grade;
			myClass.update(name, grade);
		} else if (option == 4) {
			string name;
			cout << "What is the student's name? ";
			cin >> name;
			myClass.search(name);
		}
		cout << endl << endl;
		char cont;
		cout << "Would you like to continue? (y/n)";
		cin >> cont;
		if (cont != 'y')
			break;
	}
	
}