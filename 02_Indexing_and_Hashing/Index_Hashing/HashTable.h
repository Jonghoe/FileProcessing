#pragma once
#include<vector>
#include"Bucket.h"
using namespace std;
class HashTable {

protected:
	int MASK;
	int maxLevel;
	vector<Bucket*> buckets;

public:
	HashTable();
	void insert(Student& record);
	void printTable();
	int findHash(unsigned key);

private:
	void modifyBuckets();
	void modifyMask();
	void move(int src, int dst);
};