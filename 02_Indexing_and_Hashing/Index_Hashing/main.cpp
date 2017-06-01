#include<fstream>
#include<iostream>
#include<bitset>
#include"HashTable.h"
using namespace std;
void printMenu()
{
	cout << "=================================================================================================================" << endl;
	for(int i=0;i<15;++i)
		cout << "#\t\t\t\t\t\t\t\t\t\t\t\t\t\t#\n";
	cout << "=================================================================================================================" << endl;
}
void printExit()
{
	cout << "=================================================================================================================" << endl;
	for (int i = 0; i<15; ++i)
		cout << "#\t\t\t\t\t\t\t\t\t\t\t\t\t\t#\n";
	cout << "=================================================================================================================" << endl;
}
void body()
{
	const int MAIN = 0;
	const int EXIT = 1;
	const int INPUT = 2;
	int status = MAIN;
	while (status == EXIT) {
		switch (status)
		{
		case MAIN:
			printMenu();
			break;
		case EXIT:
			printExit();
		default:
			break;
		}
	}
}
int main()
{

	vector<int> a(3,3);
	a[1]=1;
	a[2]=2;
	int* pt=&a[0];
	cout << pt[0]<<pt[1]<<pt[2]<<endl;
	printMenu();
	/*ifstream ifs("sampleData.csv",ios::in);
	string data;
	char buffer[1024];
	HashTable tlb;
	vector<Student> students;
	ifs.getline(buffer, 1024);
	int num = atoi(buffer);
	for(int i=0;i<num;++i){
		ifs.getline(buffer, 1024);
		char* buff = buffer;
		Student stu;
		int next=0;
		char *data;
		
		data = strtok(buff, ",");
		next = strlen(data);
		int j = MyStrCpy(stu.name, data);
		
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
	ifs.close();*/
	
}
