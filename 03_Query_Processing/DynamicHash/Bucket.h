#pragma once
#include<fstream>
using namespace std;
class Bucket{
protected:

public:
	virtual int getSize()const = 0;
	virtual int getCapacity()const=0;
	virtual int getBlkNum()const = 0;
	virtual int getLevel()const = 0;
	virtual int insert(void* data)=0;
	
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