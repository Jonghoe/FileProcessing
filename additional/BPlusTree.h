#pragma once
#include "Node.h"
#include <vector>
using namespace std;

class BPlusTree {
	friend class InternalNode;
private:
	bool first = true;
	Node* rootNode;
public:
	BPlusTree ();
	void insert (float score, char* find);
	vector<string> search (float lowRange, float highRange);
	void erase (float score, char* find);
	void print ();
};