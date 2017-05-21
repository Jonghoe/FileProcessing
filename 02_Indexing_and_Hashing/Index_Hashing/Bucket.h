#pragma once
#include"CommonData.h"

class Bucket {
protected:
	static int nextBlkNum;
	const int size;
	int level;
	int capacity;
	Student* records;
	int blkNum;
public:
	Bucket(int l);
	Bucket();
	int getBlkNum() {
		return blkNum;
	}
	//position�� record�� ���� �� ��ȯ
	Student erase(int position);
	int getLevel()const;
	// data�� array �������� ���� size�� �Ѿ�� -1 ��ȯ
	int insert(Student& data);
	int getCapacity()const;
	Student& operator[](int idx)const;
};