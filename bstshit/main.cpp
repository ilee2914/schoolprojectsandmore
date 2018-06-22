#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "genBST.h"

using namespace std;

int main() {
	BST<string> tree;
	
	while (true) {
		int option = 0;
		cout << "1. Add a name" << endl
			 << "2. Delete a name" << endl
			 << "3. List the contents in ascending order" << endl
			 << "4. Display the number of nodes in the tree" << endl
			 << "5. Display the number of leafs" << endl
			 << "6. Create and display a mirror image of the tree" << endl
			 << "7. Exit the system" << endl;
		cout << "Please enter your choice -> ";
		cin >> option;
		
		if (option == 1) {
			string name;
			cout << "What name would you like to add? ";
			cin >> name;
			tree.insert(name);
		} else if (option == 2) {
			string name;
			cout << "What name would you like to delete? ";
			cin >> name;
			tree.findAndDeleteByCopying(name);
		} else if (option == 3) {
			tree.inorder();
		} else if (option == 4) {
			cout << "Number of nodes in the tree : " << tree.countNodes();
		} else if (option == 5) {
			cout << "Number of leafs in the tree : " << tree.countLeafs();
		} else if (option == 6) {
			tree.reverseTree();
			tree.inorder();
		} else if (option == 7) {
			break;
		}
		cout << endl << endl;
	}
}