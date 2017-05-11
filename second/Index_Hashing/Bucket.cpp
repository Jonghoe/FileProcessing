#include"Bucket.h"
#include<cassert>
int Bucket::nextBlkNum = 0;
Bucket::Bucket():capacity(0), size(BLOCK_SIZE / sizeof(Student)),blkNum(Bucket::blkNum++)
{
	records = new Student[size];
}
Student Bucket::erase(int position)
{
	assert(capacity <= position);
	Student temp = records[capacity - 1];
	records[capacity - 1] = records[position];
	records[position] = temp;
	--capacity;
}

int Bucket::insert(Student & data)
{
	if (capacity == size)
		return -1;
	records[capacity++] = data;
	return 0;
}
