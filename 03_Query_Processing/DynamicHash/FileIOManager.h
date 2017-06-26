#pragma once
#include<fstream>
#include<iostream>
#include<bitset>
#include"b+Tree.h"
#include<vector>
#include"HashTable.h"
#include"ProfcessorBucket.h"
#include"StudentBucket.h"
using namespace std;

class FileManager {
public:
	void hashsave(const HashTable& tlb);
	vector<int> hashload(BucketFactory::Type type);
	// 디비 저장
	template<typename Type>
	void DBsave( HashTable& tlb,string path)
	{
		vector<Bucket*> buck = tlb.getBucket();
		for (auto it = buck.begin(); it != buck.end(); ++it) {
			bucketSave(*((Type*)*it), tlb.out);
		}
	}

	Bucket* bucketSave(const ProfessorBucket& bk, ofstream& wDB);
	Bucket* bucketSave(const StudentBucket& bk, ofstream& wDB);
	vector<StudentBucket*> bucketLoadAll(StudentBucket* bk, ifstream& rDB);
	vector<ProfessorBucket*> bucketLoadAll(ProfessorBucket* bk, ifstream& rDB);
	void bucketLoad(StudentBucket** bk, int blk, ifstream &rDB);
	void bucketLoad(ProfessorBucket** bk, int blk, ifstream &rDB);

	template<typename Type>
	vector<Bucket*> bucketload(HashTable& tlb)
	{
		vector<Bucket*> bks;
		int i = Type::initNum;
		Type* bk = nullptr;
		bucketLoad(&bk, i,tlb.in);
		if (bk != nullptr)
			++i;
		while (bk != nullptr) {
			bucketLoad(&bk, i,tlb.in);
			if (bk != nullptr) {
				bks.push_back(bk);
				++i;
			}
		}
		return bks;
	}

	vector<Student> readStudent(HashTable& tlb, BPlusTree& tree);
	vector<Professor> readProfessor(HashTable& tlb, BPlusTree& tree);
	bool check = false;

};