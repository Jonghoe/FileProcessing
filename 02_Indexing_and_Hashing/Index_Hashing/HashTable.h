#pragma once
#include<vector>
#include"Bucket.h"
using namespace std;
class HashTable {

protected:
	int MASK;
	int maxLevel;

	vector<int> table;
	vector<Bucket*> buckets;

public:
	HashTable();
	void insert(Student& record);
	int findHash(unsigned key);
	void printTable();
	void printBuckets();
	bool check(unsigned key);
	~HashTable();
private:
	void backTable(int hash, int blkNum, int level);
	void modifyTable();
	void modifyMask();
	void move(int src, int dst);
};