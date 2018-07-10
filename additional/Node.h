#pragma once
#include<vector>
using namespace std;
class Node {
public:
	virtual Node* insert (float score, char* id) = 0;
	virtual void erase (float score, char* id) = 0;
	virtual vector<string> search (float lowRange, float HighRange)=0;
	virtual float getFirstItem () = 0;
	virtual void print () = 0;
private:
	

};