#pragma once
#include "Node.h"
class TerminalNode:public Node {
public:
	TerminalNode ();
	virtual float getFirstItem ();
	virtual Node* insert (float score, char* id);
	virtual void erase (float score, char* id);
	virtual vector<string> search (float lowRange, float HighRange);
	virtual void print ();
private:
	void shiftContainer (int idx);
	Node* modifyNode ();

public:
	float score[292];
	char id[292][10];
private:
	int size;
	TerminalNode* nextNode;
	const int capacity = 292;
	//const int capacity = 4;

};