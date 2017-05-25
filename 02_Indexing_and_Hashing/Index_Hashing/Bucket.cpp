#include"Bucket.h"
#include<cassert>
int Bucket::nextBlkNum = 0;
Bucket::Bucket() :size(0), capacity(BLOCK_SIZE/sizeof(Student)), blkNum(nextBlkNum++), level(1)
{
	records = new Student[capacity];
}

Bucket::Bucket(int l) : size(0), capacity(BLOCK_SIZE / sizeof(Student)), blkNum(nextBlkNum++), level(l)
{
	records = new Student[capacity];
}

Student Bucket::erase(int position)
{
	assert(position < size);
	Student temp = records[position];
	records[position] = records[size - 1];
	records[size - 1] = temp;
	--size;
	return temp;
}	

int Bucket::getSize()const
{
	return size;
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
	records[size++] = data;
	return 0;
}

int Bucket::getLevel()const{
	return level;
}
