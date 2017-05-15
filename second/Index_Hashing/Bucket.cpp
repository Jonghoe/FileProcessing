#include"Bucket.h"
#include<cassert>
int Bucket::nextBlkNum = 0;
Bucket::Bucket() :capacity(0), size(4), blkNum(nextBlkNum++), level(1)
{
	records = new Student[size];
}

Bucket::Bucket(int l) : capacity(0), size(4), blkNum(nextBlkNum++), level(l)
{
	records = new Student[size];
}

Student Bucket::erase(int position)
{
	assert(position < capacity);
	Student temp = records[position];
	records[position] = records[capacity - 1];
	records[capacity - 1] = temp;
	--capacity;
	return temp;
}	

int Bucket::getCapacity()const
{
	return capacity;
}

Student& Bucket::operator[](int idx)const
{
	return records[idx];
}

int Bucket::insert(Student & data)
{
	if (capacity == size){
		++level;
		return -1;		
	}
	records[capacity++] = data;
	return 0;
}

int Bucket::getLevel()const{
	return level;
}
