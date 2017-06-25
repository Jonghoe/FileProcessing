#pragma once

class Bucket{
public:
	static int nextBlkNum;
	virtual int getSize()const=0;
	virtual int getBlkNum()const = 0;
	virtual int getLevel()const = 0;
	static void resetNextBlkNum() { nextBlkNum = 0; }
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