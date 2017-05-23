#include "HashTable.h"
#include<iostream>
HashTable::HashTable() :table(2), MASK(1), maxLevel(1),buckets(2)
{
	buckets[0] = new Bucket();
	buckets[1] = new Bucket();
	table[0] = 0;
	table[1] = 1;
}

void HashTable::insert(Student & record)
{
	int hash = findHash(record.studentID);
	int blkNum = table[hash];
	int err;
	err = buckets[blkNum]->insert(record);
	if (err == -1 ) {
		// 해당 blkNum값의 다음 block의 절반을 저장할 block number ex) 00 -> 10
		int mod = ((1 << buckets[blkNum]->getLevel() - 1) - 1)&hash;
		int half = mod | (1 << buckets[blkNum]->getLevel() - 1);
		bool needChange = maxLevel < buckets[blkNum]->getLevel();
		if (needChange) {
			modifyTable();
			maxLevel = buckets[blkNum]->getLevel();
			modifyMask();
		}
		buckets.push_back(new Bucket(buckets[blkNum]->getLevel()));								
		table[half] = buckets.size() - 1;
		backTable(half, buckets.size() - 1, buckets.back()->getLevel());
		move(mod, half);
		insert(record);
	}	
}

int HashTable::findHash(unsigned key)
{
	return key & MASK;
}

void HashTable::printTable()
{
	for (int i = 0; i < table.size(); ++i) {
		cout << "Idx[" << i << "] ="<<table[i]<<endl;		
	}	
}

void HashTable::printBuckets()
{
	for (int i = 0; i < buckets.size(); ++i) {
		cout << "==================buckets[" << i << ", " << buckets[i]->getLevel()<< " ]=================" << endl;
		for (int j = 0; j < buckets[i]->getCapacity(); ++j) {
			cout << "stu: " << (*buckets[i])[j].name << " ID: " << (*buckets[i])[j].studentID << endl;
		}
	}
}

bool HashTable::check(unsigned key)
{
	int hash = findHash(key);
	int blkNum = table[hash];
	for (int i = 0; i < buckets[blkNum]->getCapacity(); ++i) {
		if((*buckets[blkNum])[i].studentID==key)
			return true;
	}
	return false;
}

HashTable::~HashTable()
{
	
	for (int i = 0; i < buckets.size(); ++i) {
		delete buckets[i];
	}
}
void HashTable::modifyTable()
{
	table.resize(table.size() * 2);
	for (int i = table.size() / 2; i < table.size(); ++i) {
		table[i] = table[i & MASK];
	}	
}
void HashTable::backTable(int hash,int blkNum,int level)
{
	if (maxLevel <= level)
		return;	
	table[(1 << (level)) + hash] = blkNum;
	backTable(hash, blkNum, level + 1);
	backTable((1 << (level)) + hash, blkNum, level + 1);
}
void HashTable::modifyMask()
{
	MASK = (1 << maxLevel) - 1;
}

void HashTable::move(int first, int second)
{
	int src = table[first];
	int dst = table[second];
	Bucket& a = *buckets[src];
	Bucket& b = *buckets[dst];
	int i = 0;
	while( i < buckets[src]->getCapacity()) {
		int localKey = a[i].studentID&( (1 << a.getLevel()) - 1);
		if (localKey == second) {
			b.insert(a[i]);
			a.erase(i);
		}
		else {
			++i;
		}
	}
}

