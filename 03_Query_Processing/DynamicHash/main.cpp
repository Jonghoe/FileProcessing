// This is the execution file
//  that integrates db, hash, and b+tree
#pragma warning(disable : 4996)

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include"QueryProcessing.h"
#include "FileIOManager.h"
#include "b+Tree.h"

using namespace std;
const int RECORDS = 1;
const int RELEASE = 2;
const int FILES = 3;
const int PRINTDB = 4;
const int PRINTHASH = 5;
const int Query = 6;
const int EXIT = 7;
void printMenu()
{
	cout << "<<Select one of below>>" << endl;
	cout << "[1] Insert records from a file" << endl;
	cout << "[2] Release Data" << endl;
	cout << "[3] Load from 'Students.DB', 'Students.hash', and 'Students_score.idx'" << endl;
	cout << "[4] Print DB" << endl;
	cout << "[5] Print Hash" << endl;
	cout << "[6] Execute Query" << endl;
	cout << "[7] Exit" << endl;

	cout << "Insert a number : ";
}
void printRecords()
{
	cout << "Read datas from student.csv";
}
void printDB()
{
	cout << "Read from Students.DB" << endl;
}
void printReadFiles()
{
	cout << "Read datas from student.csv, Students.hash, Students_score.idx, Students.DB" << endl;
	cout << "and make a b+tree, hashTable\n"<<" Read Students.DB ";
}
int main()
{

	int selected = 0;
	BPlusTree* studentTree = NULL;
	BPlusTree* professorTree = NULL;

	HashTable* studentTable = NULL;
	HashTable* professorTable = NULL;
	FileManager fm;
	
	vector<Student> students;
	vector<Bucket*> buckets;
	vector<int> table;
	while (1) {
		printMenu();
		cin >> selected;
		std::system("cls");
		switch (selected) {
		case RECORDS:
			printRecords();
			StudentBucket::resetNextBlkNum();
			ProfessorBucket::resetNextBlkNum();
			cout << ".";

			cout << ".";
			if (studentTable != NULL)
				delete studentTable;
			studentTable = new HashTable(BucketFactory::Type::student);
			professorTable = new HashTable(BucketFactory::Type::professor);
			if (studentTree != NULL) {
				studentTree->deleteTree();
				delete studentTree;
			}
			studentTree = new BPlusTree();
			professorTree = new BPlusTree();
			cout <<endl<< "Student Data load" ;
			fm.readStudent(*studentTable, *studentTree);
			cout << "clear"<<endl;
			cout << "Professor Data load";
			fm.readProfessor(*professorTable, *professorTree);
			cout << "clear" << endl;
			cout << "Student Data save";
			fm.hashsave(*studentTable);
			cout << "clear" << endl;
			cout << "Professor Data save";
			fm.hashsave(*professorTable);
			cout << "clear" << endl;
			cout << ".";
			fm.DBsave<StudentBucket>(*studentTable, "Students.DB");
			fm.DBsave<ProfessorBucket>(*professorTable,"Professors.DB");
			cout << ".";
			//studentTree->storeTree();
			cout << "." << endl;
			cout << "Load complete" << endl;
			getch();
			break;
		case RELEASE:
			cout << "Release Data";
			if(studentTree!=NULL)
				studentTree->deleteTree();
			cout << ".";
			if (studentTree != NULL)
				delete studentTree;
			studentTree = NULL;
			cout << ".";
			studentTree = new BPlusTree();
			cout << ".";
			if (studentTable != NULL)
				delete studentTable;
			studentTable = NULL;
			cout << ".";
			studentTable = new HashTable(BucketFactory::Type::student);
			cout << "." << endl;
			cout << "Release complete" << endl;
			getch();
			break;
		case FILES:
			printReadFiles();
			cout << ".";
			StudentBucket::resetNextBlkNum();
			ProfessorBucket::resetNextBlkNum();
			buckets = fm.bucketload<StudentBucket>(*studentTable);
			cout << ".";
			table = fm.hashload(BucketFactory::Type::student);
			cout << ".";
			if (studentTable != nullptr)
				delete studentTable;
			studentTable = new HashTable(BucketFactory::Type::student, table, buckets);
			cout << "Created Student Table!!!!" << endl;
			
			cout << "Read Professors.DB";
			buckets = fm.bucketload<ProfessorBucket>(*professorTable);
			cout << ".";
			table = fm.hashload(BucketFactory::Type::professor);
			cout << "." << endl;
			
			if(professorTable!=nullptr)
				delete professorTable;
			professorTable = new HashTable(BucketFactory::Type::professor, table, buckets);
			cout << ". Created Professor Table\n";
			//studentTree->deleteTree();
			//studentTree->loadTree();
			cout << "Loading from Files complete";
			getch();
			break;
		case PRINTDB:			
			studentTable->printBucket();
			professorTable->printBucket();
			getch();
			break;
		case PRINTHASH:;
			if (studentTable != nullptr) {
				studentTable->printTable();
			}
			getch();
			break;
		case Query:
			query(*studentTree,*professorTree,*studentTable,*professorTable);
			getch();
			break;
		case EXIT:
			cout << endl << endl << "Bye my friend" << endl << endl;
			return 0;
			break;
		default:
			cout << "Wrong Number is selected." << endl;
			cout << "Please insert a number between 1 and 6." << endl << endl;
			break;
		}
		system("cls");
	}
	if (studentTree != NULL)
		studentTree->deleteTree();
	return 0;
}
