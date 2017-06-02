#include "HashTable.h"
#include"FileIOManager.h"
#include<fstream>
#include<iostream>
HashTable::HashTable() :table(2), MASK(1), maxLevel(1),buckets(2)
{
	buckets[0]=(new Bucket());
	buckets[1]=(new Bucket());
	table[0] = 0;
	table[1] = 1;
}

HashTable::HashTable(const vector<int>& tb,const vector<Bucket*>& b)
{
	table = tb;
	maxLevel = 0;

	for (; pow(2, maxLevel) < tb.size(); ++maxLevel)
		;	
	for (int i = 0; i < b.size(); ++i) {
		buckets.push_back(b[i]);
	}
}

void HashTable::insert(Student & record)
{
	// hash값을 찾고
	int hash = findHash(record.studentID);
	// hash값을 이용하여 bucket Num을 찾는다
	int blkNum = table[hash];
	// 입력때 오버플로우 발생 시 err 는 -1이 됨
	int err = buckets[blkNum]->insert(record);
	if (err == -1 ) {
		// 오버플로우난 bucket의 레벨에 맞게금 mask와 hash 조정
		int fitMask = (1 << (buckets[blkNum]->getLevel() - 1)) - 1;
		int fitHash = (fitMask)&hash;
		// 오버플로우 발생하여 새로 생길 버킷에 대한 키 값(최상위 비트에 1추가)
		int half = fitHash | (fitMask + 1);	
		// 오버플로우 발생한 버킷의 레벨이 테이블 레벨보다 크면
		// 테이블의 크기를 2배로 확장시키고 마스크 값과 테이블의 레벨을 조정한다.
		bool needChange = maxLevel < buckets[blkNum]->getLevel();
		if (needChange) {
			modifyTable();
			maxLevel = buckets[blkNum]->getLevel();
			modifyMask();
		}
		// 버킷을 생성
		buckets.push_back(new Bucket(buckets[blkNum]->getLevel()));
		// 테이블에 버킷number 저장
		table[half] = buckets.back()->getBlkNum();
		// 테이블 조정
		backTable(half, buckets.size() - 1, buckets.back()->getLevel());
		// 새로운 키값에 대한 이동
		move(fitHash, half);
		// 다시 삽입.
		insert(record);
	}	
	/*else {
		FileManager fm;
		fm.hashsave(*this);
		fm.DBsave(*this);
	}*/
}

int HashTable::findHash(unsigned key)const
{
	return key & MASK;
}

void HashTable::printTable()const
{
	for (unsigned i = 0; i < table.size(); ++i) {
		cout << "Idx[" << i << "] ="<<table[i]<<endl;		
	}	
}

void HashTable::printBuckets()const
{
	for (unsigned i = 0; i < buckets.size(); ++i) {
		cout << "==================buckets[" << i << ", " << buckets[i]->getLevel()<< " ]=================" << endl;
		for (int j = 0; j < buckets[i]->getSize(); ++j) {
			char name[21];
			int k = MyStrCpy(name, (*buckets[i])[j].name);
			if (k > 20)
				k = 20;
			name[k] = '\0';
			cout << "stu: " << name << " ID: " << (*buckets[i])[j].studentID << endl;
		}
	}
}

const vector<Bucket*>& HashTable::getBucket()const
{
	return buckets;
}

const vector<int>& HashTable::getTable()const
{
	return table;
}

unsigned HashTable::getBlkNum(int key) const
{
	return table[findHash(key)];
}

bool HashTable::check(unsigned key)const
{
	int hash = findHash(key);
	int blkNum = table[hash];
	for (int i = 0; i < buckets[blkNum]->getSize(); ++i) {
		if((*buckets[blkNum])[i].studentID==key){
			return true;
		}
	}
	return false;
}

HashTable::~HashTable()
{
	Bucket::resetNextBlkNum();
	for (int i = 0; i < buckets.size(); ++i)
		delete buckets[i];
}
void HashTable::modifyTable()
{
	//테이블의 크기를 2배로 늘리고 최상위 비트아래 비트들만 mask값을 이용
	table.resize(table.size() * 2);
	for (unsigned i = table.size() / 2; i < table.size(); ++i) {
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
	while( i < buckets[src]->getSize()) {
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
int MyStrCpy(char* dst, const char* src)
{
	int i = 0;
	for (; *src != '\0'&&i<20; ++src, ++dst,++i) {
		*dst = *src;
	}
	return i;
}