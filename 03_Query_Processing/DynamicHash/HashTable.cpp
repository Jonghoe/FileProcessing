#include "HashTable.h"
#include "FileIOManager.h"
#include<algorithm>
#include<fstream>
#include<iostream>


HashTable::HashTable() :table(2), MASK(1), maxLevel(1), buckets(), out(ofstream("Students.DB",ios::binary)), in(ifstream("Students.DB", ios::binary))
{
	createBucket(0);
	createBucket(1);
	table[0] = 0;
	table[1] = 1;
}


HashTable::HashTable(const vector<int>& tb, const vector<Bucket*>& b) : out(ofstream("Students.DB")), in(ifstream("Students.DB"))
{
	table = tb;
	maxLevel = 0;

	for (; pow(2, maxLevel) < tb.size(); ++maxLevel)
		;
	for (int i = 0; i < b.size(); ++i) {
		buckets.push_back(b[i]);
	}
}


bool HashTable::modifyBlock(int blk)
{
	
	auto it = buckets.begin();
	for (; it != buckets.end() && (*it)->getBlkNum() != blk; ++it);
	if (it != buckets.end())
		return true;
	FileManager fm;
	// �޸𸮿��� �������� ����
	if (buckets.size() >= 500){		
		FileManager fm;
		fm.bucketSave(*buckets.back(), out);
		buckets.pop_back();
	}
	Bucket* newBlk = fm.bucketLoad(blk, in);
	if (newBlk != nullptr) {
		buckets.push_back(newBlk);
		return true;
	}
	else {
		return false;
	}
}


void HashTable::createBucket(int blk)
{
	FileManager fm;
	buckets.push_back(new Bucket());
	fm.bucketSave(*buckets.front(), out);
}


void HashTable::createBucket(int blk,int level)
{
	FileManager fm;
	buckets.push_back(new Bucket(level));
	fm.bucketSave(*buckets.front(), out);
}


Bucket* HashTable::getBlock(int blk)const
{
	if (buckets[blk]->getBlkNum() == blk) {
		return buckets[blk];
	}
	CheckBlk func(blk);
	auto it = find_if(buckets.begin(), buckets.end(),func);
	if (it == buckets.end())
		return nullptr;
	return *it;
}


Bucket* HashTable::getBlock(int blk)
{
	if (buckets[blk]->getBlkNum() == blk) {
		return buckets[blk];
	}
	CheckBlk func(blk);
	auto it = find_if(buckets.begin(), buckets.end(), func);
	if(it==buckets.end())
		return nullptr;
	sort(buckets.begin(),buckets.end());
	return *it;
}


void HashTable::insert(Student& record)
{
	// hash���� ã��
	int hash = findHash(record.studentID);
	// hash���� �̿��Ͽ� bucket Num�� ã�´�
	int blkNum = table[hash];
	// �Է¶� �����÷ο� �߻� �� err �� -1�� ��
	int err = getBlock(blkNum)->insert(record);	
	if (err == -1 ) {
		// �����÷ο쳭 bucket�� ������ �°Ա� mask�� hash ����
		int fitMask = (1 << (getBlock(blkNum)->getLevel() - 1)) - 1;
		int fitHash = (fitMask)&hash;
		// �����÷ο� �߻��Ͽ� ���� ���� ��Ŷ�� ���� Ű ��(�ֻ��� ��Ʈ�� 1�߰�)
		int half = fitHash | (fitMask + 1);	
		// �����÷ο� �߻��� ��Ŷ�� ������ ���̺� �������� ũ��
		// ���̺��� ũ�⸦ 2��� Ȯ���Ű�� ����ũ ���� ���̺��� ������ �����Ѵ�.
		bool needChange = maxLevel < getBlock(blkNum)->getLevel();
		if (needChange) {
			modifyTable();
			maxLevel = getBlock(blkNum)->getLevel();
			modifyMask();
		}
		// ��Ŷ�� ����
		createBucket(blkNum, getBlock(blkNum)->getLevel());
		// ���̺� ��Ŷnumber ����
		table[half] = buckets.back()->getBlkNum();
		// ���̺� ����
		backTable(half, buckets.size() - 1, buckets.back()->getLevel());
		// ���ο� Ű���� ���� �̵�
		move(fitHash, half);
		// �ٽ� ����.
		insert(record);
	}
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
		cout << "==================buckets[" << i << ", " << getBlock(i)->getLevel()<< " ]=================" << endl;
		for (int j = 0; j < getBlock(i)->getSize(); ++j) {
			char name[21];
			int k = MyStrCpy(name, (*getBlock(i))[j].name);
			if (k > 20)
				k = 20;
			name[k] = '\0';
			cout << "stu: " << name << " ID: " << (*getBlock(i))[j].studentID << endl;
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
	for (int i = 0; i < getBlock(blkNum)->getSize(); ++i) {
		if((*getBlock(blkNum))[i].studentID==key){
			return true;
		}
	}
	return false;
}

HashTable::~HashTable()
{
	Bucket::resetNextBlkNum();
	for (int i = 0; i < buckets.size(); ++i)
		delete getBlock(i);
}


void HashTable::modifyTable()
{
	//���̺��� ũ�⸦ 2��� �ø��� �ֻ��� ��Ʈ�Ʒ� ��Ʈ�鸸 mask���� �̿�
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
	Bucket& a = *getBlock(src);
	Bucket& b = *getBlock(dst);
	int i = 0;
	while( i < a.getSize()) {
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