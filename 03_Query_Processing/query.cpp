#ifndef QUERY_CPP
#define QUERY_CPP

#include <stdio.h>
#include <iostream>
#include <string.h>
//#include "./B+Tree/b+TreeIO2.cpp"
#include"QueryProcessing.h"
using namespace std;

int float2Int(char floatIn[10]) {
  int scoreCal = floatIn[0] - '0';
  int i = 1;
  for (; floatIn[i] != '\0'; i++) {
    if (floatIn[i] == '.')
      continue;
    scoreCal = (scoreCal * 10) + (floatIn[i] - '0');
  }
  if (i == 1)
    i = 2;
  for (; i < 10; i++)
    scoreCal *= 10;
  return scoreCal;
}

char* int2Float(int intIn) {
  char* floatOut = new char [8];
  floatOut[0] = intIn/100000000 + '0';
  floatOut[1] = '.';
  floatOut[2] = intIn/10000000 + '0';
  floatOut[3] = intIn/1000000 + '0';
  floatOut[4] = intIn/100000 + '0';
  floatOut[5] = intIn/10000 + '0';
  floatOut[6] = intIn/1000 + '0';
  floatOut[7] = '\0';

  if (floatOut[6] == '0') {
    floatOut[6] = '\0';
    if (floatOut[5] == '0') {
      floatOut[5] = '\0';
      if (floatOut[4] == '0') {
	floatOut[4] = '\0';
	if (floatOut[3] == '0') {
	  floatOut[3] = '\0';
	  if (floatOut[2] == '0') {
	    floatOut[1] = '\0';
	  }
	}
      }
    }
  }

  return floatOut;
}


int query(BPlusTree& stu,BPlusTree& pro,HashTable& stuH,HashTable& proH) {
  int queryNum, temp1, temp2;
  char search[13], table[11], attr[10], temp3[10], temp4[10];
  FILE* quIn = fopen("./query.dat", "r");
  FILE* quOut = fopen("./query.res", "w");
  if (!quIn)
    printf("ERROR: Unable to find query.dat");
  if (!quOut)
    printf("ERROR: Unable to find query.res");
  int i = 1;

  fscanf(quIn, "%d\n", &queryNum);
  while(queryNum--) {
    //printf("\n%d\n", i++);
    fscanf(quIn, "%[^,], %[^,], %[^,]", search, table, attr);
    if (search[0] == 'J') {
      if ((table[0] == 'P' && attr[0] == 'S')
	  || (table[0] == 'S' && attr[0] == 'P')) {
	// 1. select * from student join professor
	printf("JOIN\n");
	JoinProcessing(quOut, NESTEDBLOCKLOOPJOIN);
      }
      else {
	printf("ERROR: Invalid table name in join\n");
      }
    }
    else if (table[0]=='S') {
      if (search[7]=='E') {
	if (attr[1]=='t') {
	  // 2. select * from student where id = XXXXXXXXX
	  fscanf(quIn, ", %d\n", &temp1);
	  printf("select * from student where id = %d", temp1);
	  FindRecord(changeID2BlkNum(stu.search(temp1, temp1),stuH),temp1,temp1,quOut,0);
	}
	else if (attr[1]=='c') {
	  // 3. select * from student where score = X.XXXXX
	  fscanf(quIn, ", %[^\n]\n", temp3);
	  int k = float2Int(temp3);
	  printf("select * from student where score = %d\n", k);
	  FindRecord(changeID2BlkNum(stu.search(k, k), stuH), k, k, quOut, 1);
	}
	else
	  printf("ERROR: Invalid attribute in students table\n");
      }
      else if (search[7]=='R') {
	if (attr[1]=='c') {
	  // 4. select * from student where score <= X.XXXXX AND X.XXXXX <= score
	  fscanf(quIn, ", %[^,], %[^\n]\n", temp3, temp4);
	  printf("select * from student where score <= %d AND %d <= score", float2Int(temp3), float2Int(temp4));
	  int b = float2Int(temp3), e = float2Int(temp4);
	  FindRecord(changeID2BlkNum(stu.search(b, e), stuH), b, e, quOut, 1);
	}
	else
	  printf("ERROR: Invalid attribute in students table\n");
      }
      else
	printf("ERROR: Invalid search in students table\n");
    }
    else if (table[0]=='P') {
      if (search[7]=='E') {
	if (attr[0]=='P') {
	  // 5. select * from professor where ProfID = XXXXXXXXX
	  fscanf(quIn, ", %d\n", &temp1);
	  printf("select * from professor where ProfID = %d", temp1);
	  FindRecord(changeID2BlkNum(pro.search(temp1, temp1), proH), temp1, temp1, quOut, 2);

	  //fscanf(quOut, "select * from professor where ProfID = %d", temp1);
	}
	else if (attr[0]=='S') {
	  // 6. select * from professor where Salary = XXXXXX
	  fscanf(quIn, ", %d\n", &temp1);
	  printf("select * from professor where Salary = %d", temp1);
	  FindRecord(changeID2BlkNum(pro.search(temp1, temp1), proH), temp1, temp1, quOut, 3);

	  //fscanf(quOut, "select * from professor where Salary = %d", temp1);
	}
	else
	  printf("ERROR: Invalid attribute in professors table\n");
      }
      else if (search[7]=='R') {
	if (attr[0]=='S') {
	  // 7. select * from professor where Salary <= XXXXXX AND Salary >= XXXXXX
	  fscanf(quIn, ", %d, %d\n", &temp1, &temp2);
	  printf("select * from professor where Salary >= %d AND Salary <= %d", temp1, temp2);
	  FindRecord(changeID2BlkNum(pro.search(temp1, temp2), proH), temp1, temp2, quOut, 3);

	  //fscanf(quOut, "select * from professor where Salary >= %d AND Salary <= %d", temp1, temp2);
	}
	else
	  printf("ERROR: Invalid attribute in professors table\n");
      }
      else
	printf("ERROR: Invalid search in professors table\n");
    }
    else {
      printf("ERROR: Invalid table name\n");
    }
  }
  fclose(quIn);
  fclose(quOut);
  return 0;
}
/*
int main() {
  query();
  return 0;
}
*/



void PrintJoinData(FILE* fp, const Student& stu, const Professor pro)
{
	fprintf(fp, "%s, %d, %lf, %d, %s, %d, %d\n", stu.name, stu.studentID, stu.score, stu.advisorID, pro.name, pro.ProfID, pro.Salary);
}
void JoinProcessing(FILE* fp, flag f)
{
FileManager fm;
if (f == NESTEDBLOCKLOOPJOIN) {

	// student
	vector<StudentBucket*> AllStudent = fm.bucketLoadAll(&StudentBucket(), ifstream("Students.DB",ios::binary));
	for (unsigned i = 0; i < AllStudent.size(); ++i) {
		ProfessorBucket* bucket = nullptr;
		fm.bucketLoad(&bucket, i + ProfessorBucket::initNum, ifstream("Professors.DB", ios::binary));
		if (bucket == nullptr)
			break;
		ProfessorBucket& professors = *bucket;
		StudentBucket& students = *AllStudent[i];
		for (int s = 0; s<students.getSize(); ++s) {
			for (int p = 0; p<professors.getSize(); ++p) {
				if (students[s].advisorID == professors[p].ProfID) {
					PrintJoinData(fp, students[s], professors[p]);
					break;
				}
			}
		}
	}
}
}

vector<int> changeID2BlkNum(int* ids, HashTable& tlb)
{
int count = ids[0];
vector<int> blkNums;
for (int i = 0; i < count; ++i) {
	blkNums.push_back(tlb.findHash(ids[i + 1]));
}
return blkNums;
}
void FindRecord(vector<int>& blknums, int min, int max, FILE* fp, int flag)
{
FileManager fm;
vector<int> bucket;
for (int i = 0; i < blknums.size(); ++i) {
	auto it = find(bucket.begin(), bucket.end(), blknums[i]);
	if (it != bucket.end())
		bucket.push_back(blknums[i]);
}
for (int i = 0; i < bucket.size(); ++i) {
	if (flag <= 1) {
		StudentBucket* stu;
		fm.bucketLoad(&stu, bucket[i], ifstream("Professors.DB", ios::binary));
		for (int k = 0; k < stu->getSize(); ++k) {
			if (flag == 0 && min == (*stu)[k].studentID)
				fscanf(fp, "%s, %d, %lf, %d\n", (*stu)[k].name, (*stu)[k].studentID, (*stu)[k].score, (*stu)[k].advisorID);
			else if (flag == 1 && min <= (*stu)[k].score * 100000000 && (*stu)[k].score * 100000000 <= max)
				fscanf(fp, "%s, %d, %lf, %d\n", (*stu)[k].name, (*stu)[k].studentID, (*stu)[k].score, (*stu)[k].advisorID);
		}
	}
	else {
		ProfessorBucket* pro;
		fm.bucketLoad(&pro, bucket[i], ifstream("Professors.DB", ios::binary));
		for (int k = 0; k < pro->getSize(); ++k) {
			if (flag == 2 && min == (*pro)[k].ProfID)
				fscanf(fp, "%s, %d, %d\n", (*pro)[k].name, (*pro)[k].ProfID, (*pro)[k].Salary);
			else if (flag == 3 && min <= (*pro)[k].Salary && (*pro)[k].Salary <= max)
				fscanf(fp, "%s, %d, %d\n", (*pro)[k].name, (*pro)[k].ProfID, (*pro)[k].Salary);
		}
	}
}
}
#endif
