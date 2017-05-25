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

	// 키값이 유요한지 확인
	bool check(unsigned key)const;
	~HashTable();
private:
	// 입력 오버 플로우가 생겼을 시 이미 늘어나 있는 테이블들 중 뒤쪽 table들을 수정 
	// ex)  hash값이 0110인 값이 들어오고 0010이 오버 플로우 나면서 0110에 bucket number가 4인 bucket 생성
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
	
	// bucket의 level중 현재까지 최대 level보다 큰 bucket이 생기면 table의 수를 2배로 늘려준다.
	void modifyTable();

	// MASK field의 값을 변경
	void modifyMask();
	
	// blocking factor를 넘어서 block을 둘로 나눌때 사용
	void move(int src, int dst);
};