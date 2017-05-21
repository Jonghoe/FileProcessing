#include<iostream>
#include<fstream>
#include"HashTable.h"
using namespace std;

int main()
{
	ifstream ifs("data.txt");
	string data;
	char buff[1024];
	HashTable tlb;
	vector<Student> students;
	while(!ifs.eof()){
		ifs.getline(buff, 1024);
		Student stu;
		sscanf(buff, "%s %d", stu.name, &stu.studentID);
		students.push_back(stu);
	}
	
	for (int i = 0; i < students.size(); ++i)
		tlb.insert(students[i]);	

	tlb.printTable();
	tlb.printBuckets();
}