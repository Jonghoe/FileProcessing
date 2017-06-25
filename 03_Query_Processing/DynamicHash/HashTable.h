#pragma once
#include<vector>
#include<fstream>
#include"Bucket.h"
#include"CommonData.h"
#include"BucketFactory.h"
using namespace std;
class HashTable {
protected:
	BucketFactory::Type type;
	int MASK;
	int maxLevel;
	vector<int> table;	
	vector<Bucket*> buckets;
public:
	HashTable(BucketFactory::Type t );
	HashTable(BucketFactory::Type t,const vector<int>& tb, const vector<Bucket*>& b);
	void insert(Student& record);
	int findHash(unsigned key)const;
	void printTable()const;
	const vector<Bucket*>& getBucket()const;
	const vector<int>& getTable()const;
	unsigned getBlkNum(int key)const;
	
	// Ű���� �������� Ȯ��
	bool check(unsigned key)const;
	~HashTable();
private:
	// �Է� ���� �÷ο찡 ������ �� �̹� �þ �ִ� ���̺�� �� ���� table���� ���� 
	// ex)  hash���� 0110�� ���� ������ 0010�� ���� �÷ο� ���鼭 0110�� class DataType number�� 4�� class DataType ����
	//		level: 2		level: 3
	//		0000 -> 0		0000 -> 0
	//		0010 -> 1		0010 -> 1
	//		0100 -> 0		0100 -> 0
	//		0110 -> 1	=>	0110 -> 4
	//		1000 -> 0		1000 -> 0
	//		1100 -> 0		1100 -> 0
	//		1110 -> 1		1110 -> 4
	//		1010 -> 1		1010 -> 1
	
	void backTable(int hash, int blkNum, int level);
	//  DataType�� level�� ������� �ִ� level���� ū class DataType�� ����� table�� ���� 2��� �÷��ش�.
	void modifyTable();

	// MASK field�� ���� ����
	void modifyMask();
	
	// blocking factor�� �Ѿ block�� �ѷ� ������ ���
	void move(int src, int dst);
	// block �޸𸮿� �ִ��� Ȯ���Ͽ� �޸𸮿� ������ ���� �б�
	Bucket* getBlock(int blk);
	Bucket* getBlock(int blk)const;
	bool modifyBlock(int blk);
	void createBucket(int blk);
	void createBucket(int blk, int level);
	void bucketSave();
};

int MyStrCpy(char* dst, const char* src);