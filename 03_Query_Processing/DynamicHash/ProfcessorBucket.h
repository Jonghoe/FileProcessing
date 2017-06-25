#pragma once
#include"Bucket.h"
#include"CommonData.h"
class ProfessorBucket :public Bucket{
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
	//position�� record�� ���� �� ��ȯ
	Professor erase(int position);
	int getLevel()const;
	// data�� array �������� ���� size�� �Ѿ�� -1 ��ȯ
	int insert(void* data);
	int getSize()const;
	int getCapacity()const;
	Professor& operator[](int idx)const { return records[idx]; };

};