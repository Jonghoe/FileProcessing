#pragma once
#include<fstream>
#include<iostream>
#include<bitset>
#include"b+Tree.h"
#include"b+Tree.h"
using namespace std;

class FileManager {
public:
	void hashsave(const HashTable& tlb);
	vector<int> hashload();
	// ��� ����
	void DBsave( HashTable& tlb);
	Bucket* bucketSave(const Bucket& bk, ofstream& wDB);
	Bucket* bucketLoad(int blk, ifstream& rDB);
	vector<Bucket*> bucketload();
	vector<Student> readStudent(HashTable& tlb, BPlusTree& tree);
	vector<Student> readProfessor(HashTable& tlb, BPlusTree& tree);

};