 #pragma once
#include"CommonData.h"
#include"Bucket.h"
#pragma pack(push,1)
class StudentBucket :public Bucket{
protected:
	static int nextBlkNum;
	const int capacity;
	int level;
	int size;
	Student* records;
	int blkNum;
public:
	StudentBucket(int l);
	StudentBucket();
	int getBlkNum() const{
		return blkNum;
	}
	static void resetNextBlkNum() { nextBlkNum = initNum; }
	//position의 record를 삭제 및 반환
	Student erase(int position);
	int getLevel()const;
	// data를 array 마지막에 삽입 size를 넘어가면 -1 반환
	int insert(void* data);
	int getSize()const;
	int getCapacity()const;
	Student& operator[](int idx)const { return records[idx]; };
	const static int initNum =0;
};
#pragma pack(pop)