#pragma once
#include"Bucket.h"
#include"CommonData.h"
class ProfessorBucket :public Bucket{
	static int nextBlkNum;
protected:
	const int capacity;
	int level;
	int size;
	Professor* records;
	int blkNum;
public:
	ProfessorBucket(int l);
	ProfessorBucket();
	int getBlkNum() const
	{
		return blkNum;
	}
	//position의 record를 삭제 및 반환
	Professor erase(int position);
	int getLevel()const;
	// data를 array 마지막에 삽입 size를 넘어가면 -1 반환
	int insert(void* data);
	int getSize()const;
	int getCapacity()const;
	Professor& operator[](int idx)const { return records[idx]; };
	const static int initNum=10000000;
	static void resetNextBlkNum() { nextBlkNum = initNum; }
};