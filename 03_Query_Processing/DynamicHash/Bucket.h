#pragma once
#include<fstream>
using namespace std;
class Bucket{
	ofstream out;
	ifstream in;
public:
	static int nextBlkNum;
	virtual int getSize()const = 0;
	virtual int getCapacity()const=0;
	virtual int getBlkNum()const = 0;
	virtual int getLevel()const = 0;
	virtual int insert(void* data)=0;
	static void resetNextBlkNum() { nextBlkNum = 0; }
	ofstream& getOut() { return out; }
	ifstream& getIn() { return in; }
};


class CheckBlk {
	int comp;
public:
	CheckBlk(int c) :comp(c) {}
	bool operator()(const Bucket* buck)
	{
		return buck->getBlkNum() == comp;
	}
};