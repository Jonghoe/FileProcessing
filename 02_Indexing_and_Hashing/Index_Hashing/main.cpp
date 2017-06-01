#include<fstream>
#include<iostream>
#include<bitset>
#include"HashTable.h"
using namespace std;

int main()
{
	
	ifstream ifs("./sampleData.csv");
	string data;
	char buffer[1024];
	HashTable tlb;
	vector<Student> students;
	ifs.getline(buffer, 1024);
	int num = atoi(buffer);
	// 버킷 넘버 데이터 수 버킷 레벨
	const vector<Bucket*> buck = tlb.getBucket();
	string filePath = "Student.DB";
	ofstream writeFile(filePath.data(), ios::binary);
	for(int i = 0; i<buck.size(); ++i) {
		writeFile << bitset<32>(buck[i]->getBlkNum())<< ' ';
		writeFile << bitset<32>(buck[i]->getSize())<<' ';
		writeFile << bitset<32>(buck[i]->getLevel())<<endl;;
		const Bucket& b = *buck[i];
		for(int j = 0; j<buck[0]->getSize(); ++j) {
			writeFile << bitset<32>(b[j].advisorID) <<' ';
			writeFile << b[j].name << ' ';
			writeFile << bitset<32>(b[j].score)<< ' ';
			writeFile << bitset<32>(b[j].studentID)<<endl;
		}
	}
	writeFile.close();
//	tlb.
	const vector<int>& tb = tlb.getTable();
	
	for(int i=0;i<num;++i){
		ifs.getline(buffer, 1024);
		char* buff = buffer;
		Student stu;
		int next=0;
		char *data;
		
		data = strtok(buff, ",");
		strcpy(stu.name,data);
		next = strlen(data);
		buff = buff + next+1;

		data = strtok(buff, ",");
		stu.studentID=atoi(data);
		next = strlen(data);
		buff = buff + next+1;

		data = strtok(buff, ",");
		stu.score = atof(data);
		next = strlen(data);
		buff = buff + next + 1;

		data = strtok(buff, ",");
		stu.advisorID=atoi(data);		

		students.push_back(stu);
		tlb.insert(students[i]);
	}
	
	ifs.close();
	cout << "==================================================" << endl;
	tlb.printBuckets();
	cout << "##################################################" << endl;
	/*
	cout << "##################################################" << endl;
	for (int i = 0; i < students.size(); ++i) {
		if (tlb.check(students[i].studentID))
			cout << bitset<32>(students[i].studentID) << ": " << tlb.findHash(students[i].studentID) << endl;
	}
	*/
}