#include<fstream>
#include<iostream>
#include<bitset>
#include"HashTable.h"

using namespace std;
class Filemanager {
public:
	void Filemanager::hashsave(const HashTable& tlb)
	{
		const vector<int>& tb = tlb.getTable();
		string hashPath = "Students.hash";
		ofstream writeHash(hashPath.data(), ios::binary);
		for(int i = 0; tb.size(); ++i)
			writeHash<<i<<bitset<32>(tb[i])<<endl;
		writeHash.close();
	}
	vector<int> Filemanager::hashload()
	{
		int i, temp;
		vector<int> ht;
		string hashPath = "Students.hash";
		ifstream hashl(hashPath.data(), ios::binary);
		while(!hashl.eof()) {
			hashl>>i>>temp;
			ht.push_back(temp);
		}
		return ht;
	}
	void Filemanager::DBsave(const HashTable& tlb)
	{
		const vector<Bucket*> buck = tlb.getBucket();
		string filePath = "Student.DB";
		ofstream writeFile(filePath.data(), ios::binary);
		for(int i = 0; i<buck.size(); ++i) {
			writeFile<<bitset<32>(buck[i]->getBlkNum());
			writeFile<<bitset<32>(buck[i]->getSize());
			writeFile<<bitset<32>(buck[i]->getLevel());
			const Bucket& b = *buck[i];
			for(int j = 0; j<buck[0]->getSize(); ++j) {
				writeFile<<bitset<32>(b[j].advisorID);
				writeFile<<b[j].name<<' ';
				writeFile<<bitset<32>(b[j].score);
				writeFile<<bitset<32>(b[j].studentID);
			}
		}
		writeFile.close();
	}
	vector<Bucket*> Filemanager::bucketload()
	{
		int t_bln, t_size, t_l, t_aid, t_score, t_sid;
		char* t_n;
		vector<Bucket*> bks;
		string hashPath = "Student.DB";
		ifstream rdb(hashPath.data(), ios::binary);
		while(!rdb.eof()) {
			rdb>>t_bln>>t_size>>t_l;
			Bucket* bk = new Bucket();
			for(int i = 0; i<t_size; ++i) {
				rdb>>t_aid>>t_n>>t_score>>t_sid;
				Student stu;
				stu.advisorID = t_aid;
				strcpy(stu.name,t_n);
				stu.score = t_score;
				stu.studentID = t_sid;
				bk->insert(stu);

			}
		}
		return bks;
	}
};