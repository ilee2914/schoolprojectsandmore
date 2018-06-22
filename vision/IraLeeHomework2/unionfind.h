// Ira Lee

#ifndef UNION_FIND
#define UNION_FIND

#include <cstdlib>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Node{
	int value = -1;
	Node* prev = nullptr;
	vector<Node*> next;
};
	
class UnionFind {
public:
	Node* find(int num) {
		Node* node;
		auto it = nodeLocs.find(num);
		
		if (it != nodeLocs.end()) {		//If node exists
			node = it->second;
		} else {						//Create a new node if doesn't
			return add(num);
		}
		
		if(node->prev == nullptr)		//if is root
		return node;
		Node* temp = node;
		while (temp->prev != nullptr)		//temp now equals root of its tree
		temp = temp->prev;
		if (node->prev == temp)				//If node was already connected to root
		return temp;
		temp->next.push_back(node);			//ISets node as child to parent
		vector<Node*> x = node->prev->next;
		for (int i = 0; i < x.size(); i++) {	//Removes link from previous parent
			if (x[i]->value == node->value) {
				x.erase(x.begin() + i);
				break;
			}
		}
		if (node == temp) {
			return node;
		}
		node->prev = temp;			//Sets new parent to root
		return temp;
	}
	
	bool Union(int a, int b) {
		Node* aRoot = find(a);
		
		Node* bRoot = find(b);
		if (aRoot == nullptr || bRoot == nullptr) {
			return false;
		}
		if (aRoot == bRoot) {			//already connected
			return true;
		}
		aRoot->next.push_back(bRoot);
		bRoot->prev = aRoot;
		
		for (int i = 0; i < collection.size(); i++) {		//Remove the root from the collection
			if (collection[i]->value == bRoot->value) {
				collection.erase(collection.begin() + i);
				break;
			}
		}
		return true;
	}
	
	void changeValue(Node * a, int num) {
		a->value = num;
	}
	
	void printRoots() {
		for (int i = 0; i < collection.size(); i++) {
			cout << collection[i]->value<< " ";
		}
	}

	vector<Node*> getCollection() {return collection;}
	
private:
	vector<Node*> collection;				//collection of roots
	unordered_map<int, Node*> nodeLocs;		//location of nodes for easy find for values
	
	//Adds new node if doesn't exist.
	Node* add(int num) {
		if (nodeLocs.find(num) == nodeLocs.end()) {
			Node* newNode = new Node();
			newNode->value = num;
			collection.push_back(newNode);
			nodeLocs.insert({newNode->value, newNode});
			return newNode;
		}
		return nullptr;
	}
};

#endif