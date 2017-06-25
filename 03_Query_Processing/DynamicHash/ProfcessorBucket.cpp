#include "ProfcessorBucket.h"
#include<fstream>
#include<iostream>
#include<cassert>
using namespace std;
int ProfessorBucket::nextBlkNum = initNum;

ProfessorBucket::ProfessorBucket() :size(0), capacity((BLOCK_SIZE - 12) / sizeof(Professor)), blkNum(nextBlkNum++), level(1)
{
	records = new Professor[capacity];
}

ProfessorBucket::ProfessorBucket(int l) : size(0), capacity((BLOCK_SIZE - 12) / sizeof(Professor)), blkNum(nextBlkNum++), level(l)
{
	records = new  Professor[capacity];
}

Professor ProfessorBucket::erase(int position)
{
	// ���� ���� ����� �����ϰ� size�� ���δ�.
	assert(position < size);
	Professor temp = records[position];
	records[position] = records[size - 1];
	records[size - 1] = temp;
	--size;
	return temp;
}

int ProfessorBucket::getSize()const
{
	return size;
}

int ProfessorBucket::getCapacity() const
{
	return capacity;
}

int ProfessorBucket::insert(void* data)
{
	// size�� capacity�� ������ �����÷ο�
	if (capacity == size) {
		++level;
		return -1;
	}
	records[size++] = *(Professor*)data;
	return 0;
}



int ProfessorBucket::getLevel()const
{
	return level;
}
