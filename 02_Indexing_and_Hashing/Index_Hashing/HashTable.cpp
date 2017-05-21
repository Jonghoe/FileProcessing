#include "HashTable.h"
#include<iostream>
HashTable::HashTable() :buckets(2), MASK(1), maxLevel(1)
{
	buckets[0] = new Bucket();
	buckets[1] = new Bucket();
}

void HashTable::insert(Student & record)
{
	int hash = findHash(record.studentID);
	int err;
	err = buckets[hash]->insert(record);
	if (err == -1 ) {
		// 해당 hash값의 다음 block의 절반을 저장할 block number ex) 00 -> 10
		int half = ((1 << buckets[hash]->getLevel() - 1) - 1)&hash |( 1 << buckets[hash]->getLevel() - 1);
		if (maxLevel < buckets[hash]->getLevel()) {		
			modifyBuckets();
			maxLevel = buckets[hash]->getLevel();
			modifyMask();
		}
		buckets[half] = new Bucket(buckets[hash]->getLevel());
		move(hash, half);
		insert(record);
	}
}

int HashTable::findHash(unsigned key)
{
	return key & MASK;
}

void HashTable::modifyBuckets()
{
	buckets.resize(buckets.size() * 2);
	for (int i = buckets.size() / 2; i < buckets.size(); ++i) {
		buckets[i] = buckets[i & MASK];
	}
}

void HashTable::modifyMask()
{
	MASK = (1<<maxLevel) -1 ;
}

void HashTable::printTable()
{
	for (int i = 0; i < buckets.size(); ++i) {
		cout << "=========== BlockNum: " << buckets[i]->getBlkNum() << "//Level: "<< buckets[i]->getLevel()<<" ================" << endl;
		for (int j = 0; j < buckets[i]->getCapacity(); ++j)
			cout << "ID " << (*buckets[i])[j].name << "\t" << (*buckets[i])[j].studentID << endl;
		cout << "==============================================================" << endl;
	}
}

void HashTable::move(int src, int dst)
{
	Bucket& a = *buckets[src];
	Bucket& b = *buckets[dst];
	int i = 0;
	while( i < buckets[src]->getCapacity()) {
		int localKey = a[i].studentID&((1 << a.getLevel()) - 1);
		int  as = a[i].studentID;
		if (localKey == dst) {
			b.insert(a[i]);
			a.erase(i);
		}
		else {
			++i;
		}
	}
}
