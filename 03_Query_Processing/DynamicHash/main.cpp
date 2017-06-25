// This is the execution file
//  that integrates db, hash, and b+tree
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "FileIOManager.h"
#include "b+Tree.h"
using namespace std;
const int RECORDS = 1;
const int RELEASE = 2;
const int FILES = 3;
const int PRINTDB = 4;
const int PRINTHASH = 5;
const int PRINTkTH = 6;
const int EXIT = 7;
void printMenu()
{
	cout << "<<Select one of below>>" << endl;
	cout << "[1] Insert records from a file" << endl;
	cout << "[2] Release Data" << endl;
	cout << "[3] Load from 'Students.DB', 'Students.hash', and 'Students_score.idx'" << endl;
	cout << "[4] Print DB" << endl;
	cout << "[5] Print Hash" << endl;
	cout << "[6] Print kth leaf node" << endl;
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
	cout << "and make a b+tree, hashTable";
}
int main()
{

	int selected = 0, k;
	BPlusTree* studentTree = NULL;
	FileManager fm;
	HashTable* studentTable = NULL;
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
			cout << ".";

			cout << ".";
			if (studentTable != NULL)
				delete studentTable;
			studentTable = new HashTable();
			if (studentTree != NULL) {
				studentTree->deleteTree();
				delete studentTree;
			}
			studentTree = new BPlusTree();
			fm.readStudent(*studentTable, *studentTree);
			cout << ".";
			fm.hashsave(*studentTable);
			cout << ".";
			fm.DBsave(*studentTable);
			cout << ".";
			studentTree->storeTree();
			cout << "." << endl;
			cout << "Load complete" << endl;
			getch();
			break;
		case RELEASE:
			cout << "Release Data";
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
			studentTable = new HashTable();
			cout << "." << endl;
			cout << "Release complete" << endl;
			getch();
			break;
		case FILES:
			printReadFiles();
			cout << ".";
			buckets = fm.bucketload();
			cout << ".";
			table = fm.hashload();
			cout << ".";
			delete studentTable;
			studentTable = new HashTable(table, buckets);
			cout << ".";
			studentTree->deleteTree();
			studentTree->loadTree();
			cout << "." << endl;
			cout << "Loading from Files complete";
			getch();
			break;
		case PRINTDB:
			buckets = fm.bucketload();
			for (int i = 0; i < buckets.size(); ++i) {
				cout << "Bucekt Num: " << buckets[i]->getBlkNum() << ", Bit Level: " << buckets[i]->getLevel() << endl;
				for (int j = 0; j < buckets[i]->getSize(); ++j) {
					char name[21];
					int k = MyStrCpy(name, (*buckets[i])[j].name);
					if (k <= 20)
						name[k] = '\0';
					else
						name[20] = '\0';
					cout << "name: " << name << ",\tscore: " << (*buckets[i])[j].score << ",\tstudent ID: " << (*buckets[i])[j].studentID << ",\tadvID: " << (*buckets[i])[j].advisorID << endl;
				}
			}
			getch();
			break;
		case PRINTHASH:;
			table = fm.hashload();
			for (int i = 0; i < table.size(); ++i) {
				cout << "table IDX[" << i << "] = " << table[i] << endl;
			}
			getch();
			break;
		case PRINTkTH:
			cout << "Enter number k : ";
			cin >> k;
			studentTree->findKthTerminal(k)->print(0);

			table = fm.hashload();
			for (int i = 0; i < table.size(); ++i) {
				cout << "table IDX[" << i << "] = " << table[i] << endl;
			}
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
