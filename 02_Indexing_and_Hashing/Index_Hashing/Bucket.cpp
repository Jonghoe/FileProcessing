#include<fstream>
#include<iostream>
#include"Bucket.h"
#include<cassert>
using namespace std;
int Bucket::nextBlkNum = 0;
Bucket::Bucket() :size(0), capacity((BLOCK_SIZE)/sizeof(Student)), blkNum(nextBlkNum++), level(1)
{
	records = new Student[capacity];
}

Bucket::Bucket(int l) : size(0), capacity((BLOCK_SIZE-4) / sizeof(Student)), blkNum(nextBlkNum++), level(l)
{
	records = new Student[capacity];
}

Student Bucket::erase(int position)
{	
	// 가장 뒤의 멤버와 스왑하고 size를 줄인다.
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

int Bucket::getCapacity() const
{
	return capacity;
}

Student& Bucket::operator[](int idx)const
{
	return records[idx];
}

int Bucket::insert(Student & data)
{
	// size와 capacity가 같으면 오버플로우
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
