#pragma once
#include"CommonData.h"

class Bucket {
protected:
	static int nextBlkNum;
	const int capacity;
	int level;
	int size;
	Student* records;
	int blkNum;
public:
	Bucket(int l);
	Bucket();
	int getBlkNum() {
		return blkNum;
	}
	//position의 record를 삭제 및 반환
	Student erase(int position);
	int getLevel()const;
	// data를 array 마지막에 삽입 size를 넘어가면 -1 반환
	int insert(Student& data);
	int getSize()const;
	Student& operator[](int idx)const;
	void save();
};