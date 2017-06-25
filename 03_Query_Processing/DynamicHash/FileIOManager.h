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
	vector<int> hashload();
	// ��� ����
	template<typename Type>
	void DBsave( HashTable& tlb,string path)
	{
		vector<Bucket*> buck = tlb.getBucket();
		ofstream writeFile(path.data(), ios::binary);
		for (auto it = buck.begin(); it != buck.end(); ++it) {
			bucketSave(*((Type*)*it), (*it)->getOut());
		}
		writeFile.close();
	}

	Bucket* bucketSave(const ProfessorBucket& bk, ofstream& wDB);
	Bucket* bucketSave(const StudentBucket& bk, ofstream& wDB);

	void bucketLoad(StudentBucket* bk, int blk, ifstream &rDB);
	void bucketLoad(ProfessorBucket* bk, int blk, ifstream &rDB);

	template<typename Type>
	vector<Bucket*> bucketload(string path)
	{
		float t_score;
		char t_n[21];
		vector<Bucket*> bks;
		int i = 0;
		Type* bk = nullptr;
		bucketLoad(bk, i, ifstream(path, ios::binary));
		while (bk != nullptr) {
			bucketLoad(bk, i, ifstream(path, ios::binary));
			if (bk != nullptr) {
				bks.push_back(bk);
				++i;
			}
		}
		return bks;
	}

	vector<Student> readStudent(HashTable& tlb, BPlusTree& tree);
	vector<Professor> readProfessor(HashTable& tlb, BPlusTree& tree);

};