#include<fstream>
#include<iostream>
#include"StudentBucket.h"
#include<cassert>
using namespace std;
StudentBucket::StudentBucket():size(0), capacity((BLOCK_SIZE-12)/sizeof(Student)), blkNum(nextBlkNum++), level(1)
{
	records = new Student[capacity];
}

StudentBucket::StudentBucket(int l) : size(0), capacity((BLOCK_SIZE - 12) / sizeof(Student)), blkNum(nextBlkNum++), level(l)
{

	records = new  Student[capacity];
}

Student StudentBucket::erase(int position)
{	
	// 가장 뒤의 멤버와 스왑하고 size를 줄인다.
	assert(position < size);	
	Student temp = records[position];
	records[position] = records[size - 1];
	records[size - 1] = temp;
	--size;
	return temp;
}	

int StudentBucket::getSize()const
{
	return size;
}

int StudentBucket::getCapacity() const
{
	return capacity;
}


int StudentBucket::insert(void* data)
{		
	// size와 capacity가 같으면 오버플로우
	if (capacity == size){
		++level;
		return -1;		
	}
	records[size++] = *(Student*)data;
	return 0;
}



int StudentBucket::getLevel()const{
	return level;
}
