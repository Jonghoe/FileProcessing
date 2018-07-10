#pragma once
#include"Node.h"
#include<list>
#include"BPlusTree.h"
using namespace std;
class InternalNode : public Node {
private:
	const int capacity = 511;
	//const int capacity = 4;
	list<float> scoreL;
	list<Node*> branchL;
	float getFirstItem ();	
	void overflowSolve (list<Node*>::iterator branchLoc, list<float>::iterator scoreLocation, Node* temp);

public:
	InternalNode ();
	InternalNode (Node* left, float separator, Node* right);
	
	virtual void print ();
	virtual Node* insert (float score, char* id);
	virtual void erase (float score, char* id);
	virtual vector<string> search (float lowRange, float HighRange);

private:
	Node * modifyNode ();
};