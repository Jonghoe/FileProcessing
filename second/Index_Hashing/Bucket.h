#pragma once
#include"CommonData.h"

class Bucket {
protected:
	static int nextBlkNum;
	const int size;
	int capacity;
	Student* records;
	int blkNum;
public:
	Bucket();

	//position의 record를 삭제 및 반환
	Student erase(int position);

	// data를 array 마지막에 삽입 size를 넘어가면 -1 반환
	int insert(Student& data);
	
};