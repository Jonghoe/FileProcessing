 #pragma once
#include"CommonData.h"
#include"Bucket.h"
class StudentBucket :public Bucket{
protected:
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
	//position�� record�� ���� �� ��ȯ
	Student erase(int position);
	int getLevel()const;
	// data�� array �������� ���� size�� �Ѿ�� -1 ��ȯ
	int insert(void* data);
	int getSize()const;
	int getCapacity()const;
	Student& operator[](int idx)const { return records[idx]; };
};