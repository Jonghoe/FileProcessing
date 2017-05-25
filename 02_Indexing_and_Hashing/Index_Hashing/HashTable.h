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
	int findHash(unsigned key)const;
	void printTable()const;
	void printBuckets()const;

	// Ű���� �������� Ȯ��
	bool check(unsigned key)const;
	~HashTable();
private:
	// �Է� ���� �÷ο찡 ������ �� �̹� �þ �ִ� ���̺�� �� ���� table���� ���� 
	// ex)  hash���� 0110�� ���� ������ 0010�� ���� �÷ο� ���鼭 0110�� bucket number�� 4�� bucket ����
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
	
	// bucket�� level�� ������� �ִ� level���� ū bucket�� ����� table�� ���� 2��� �÷��ش�.
	void modifyTable();

	// MASK field�� ���� ����
	void modifyMask();
	
	// blocking factor�� �Ѿ block�� �ѷ� ������ ���
	void move(int src, int dst);
};