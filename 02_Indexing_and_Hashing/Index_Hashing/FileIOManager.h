#pragma once
#include<fstream>
#include<iostream>
#include<bitset>
#include"HashTable.h"

using namespace std;
class FileManager {
public:
	void FileManager::hashsave(const HashTable& tlb)
	{
		const vector<int>& tb = tlb.getTable();
		string hashPath = "Students.hash";
		ofstream writeHash(hashPath.data(), ios::binary);
		for (int i = 0; i < tb.size(); ++i) {
			writeHash.write((char*)&i, 4);
			writeHash.write((char*)&tb[i], 4);
		}
		writeHash.close();
	}
	vector<int> FileManager::hashload()
	{
		int i, temp;
		vector<int> ht;
		string hashPath = "Students.hash";
		ifstream hashl(hashPath.data(), ios::binary);
		do{
			hashl.read((char*)&i, 4);
			hashl.read((char*)&temp,4);
			ht.push_back(temp);
		} while (!hashl.eof());
		ht.pop_back();
		return ht;
	}
	void FileManager::DBsave(const HashTable& tlb)
	{
		const vector<Bucket*> buck = tlb.getBucket();
		string filePath = "Student.DB";
		ofstream writeFile(filePath.data(), ios::binary);
		for (int i = 0; i<buck.size(); ++i) {
			int num = buck[i]->getBlkNum();
			int size = buck[i]->getSize();
			int level = buck[i]->getLevel();
			unsigned aid;
			float score;
			unsigned sid;
			writeFile.write((char*)(&num),4);
			writeFile.write((char*)&size,4);
			writeFile.write((char*)&level,4);
			const Bucket& b = *buck[i];
			for (int j = 0; j<b.getSize(); ++j) {
				aid = b[j].advisorID;
				score = b[j].score;
				sid = b[j].studentID;
				writeFile.write((char*)&aid,4);
				writeFile.write(b[j].name,20);
				writeFile.write((char*)&sid, 4);
				writeFile.write((char*)&score,4);
			}
		}
		writeFile.close();
	}
	vector<Bucket*> FileManager::bucketload()
	{
		unsigned t_bln, t_size, t_l, t_aid, t_sid;
		float t_score;
		char t_n[21];
		vector<Bucket*> bks;
		string hashPath = "Student.DB";
		ifstream rdb(hashPath.data(), ios::binary);
		do {			
			rdb.read((char*)&t_bln, 4);
			rdb.read((char*)&t_size, 4);
			rdb.read((char*)&t_l, 4);
			Bucket* bk = new Bucket();
			for (int i = 0; i<t_size; ++i) {
				rdb.read((char*)&t_aid, 4);
				rdb.read(t_n,20);
				rdb.read((char*)&t_sid, 4);
				rdb.read((char*)&t_score, 4);
				Student stu;
				stu.advisorID = t_aid;
				int len =MyStrCpy(stu.name, t_n);
				if(len<20)
					stu.name[len] = '\0';
				stu.score = t_score;
				stu.studentID = t_sid;
				bk->insert(stu);
			}
			bks.push_back(bk);
		} while (!rdb.eof());
		bks.pop_back();
		return bks;
	}
	vector<Student> FileManager::readcsv()
	{
		vector<Student> students;
		ifstream ifs("sampleData.csv");
		int num;
		char buffer[1024];
		ifs >> num;
		ifs.getline(buffer,1024);
		for (int i = 0; i<num; ++i) {
			ifs.getline(buffer, 1024);
			char* buff = buffer;
			Student stu;
			int next = 0;
			char *data;
			
			data = strtok(buff, ",");
			strcpy(stu.name, data);
			next = strlen(data);
			buff = buff + next + 1;

			data = strtok(buff, ",");
			stu.studentID = atoi(data);
			next = strlen(data);
			buff = buff + next + 1;

			data = strtok(buff, ",");
			stu.score = atof(data);
			next = strlen(data);
			buff = buff + next + 1;

			data = strtok(buff, ",");
			stu.advisorID = atoi(data);

			students.push_back(stu);
		}

		ifs.close();

		return students;
	}
};