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

	//position�� record�� ���� �� ��ȯ
	Student erase(int position);

	// data�� array �������� ���� size�� �Ѿ�� -1 ��ȯ
	int insert(Student& data);
	
};