#ifndef QUERY_CPP
#define QUERY_CPP

#include <stdio.h>
#include <iostream>
#include <string.h>

#include "./B+Tree/b+Tree.cpp"
#include "./B+Tree/b+TreeSearch.cpp"
#include "./B+Tree/b+TreeInsert.cpp"

#include "./B+Tree/b+TreeIO2.cpp"

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


int query(BPlusTree* stud, BPlusTree* prof ) {
  int queryNum, temp1, temp2;
  char search[13], table[11], attr[7], temp3[10], temp4[10];
  FILE* quIn = fopen("./query.dat", "r");
  FILE* quOut = fopen("../03_Query_Processing/queryTemp.txt", "w");
  if (!quIn)
    printf("ERROR: Unable to find query.dat");
  if (!quOut)
    printf("ERROR: Unable to find query.dat");
  int i = 1;

  fscanf(quIn, "%d\n", &queryNum);
  while(queryNum--) {
    //printf("\n%d\n", i++);
    fscanf(quIn, "%[^,], %[^,], %[^,]", search, table, attr);
    if (search[0] == 'J') {
      if ((table[0] == 'P' && attr[0] == 'S')
	  || (table[0] == 'S' && attr[0] == 'P')) {
	// 1. select * from student join professor
	//printf("JOIN\n");
	//fprintf(quOut, "JOIN\n");
	fprintf(quOut, "0\n");
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
	  //printf("select * from student where id = %d", temp1);
	  //fprintf(quOut, "select * from student where id = %d", temp1);
	  fprintf(quOut, "1\n");
	}
	else if (attr[1]=='c') {
	  // 3. select * from student where score = X.XXXXX
	  fscanf(quIn, ", %[^\n]\n", temp3);
	  
	  int* result = stud->search(float2Int(temp3), float2Int(temp3));
	  int  matchSize = result[0];
	  fprintf(quOut, "%d\n", matchSize);
	  for (int i = 1; i <= matchSize; i++)
	    fprintf(quOut, "%d\n", result[i]);
	}
	else
	  printf("ERROR: Invalid attribute in students table\n");
      }
      else if (search[7]=='R') {
	if (attr[1]=='c') {
	  // 4. select * from student where score <= X.XXXXX AND X.XXXXX <= score
	  fscanf(quIn, ", %[^,], %[^\n]\n", temp3, temp4);
	  //printf("select * from student where score <= %d AND %d <= score", float2Int(temp3), float2Int(temp4));
	  //fprintf(quOut, "select * from student where score <= %d AND %d <= score", float2Int(temp3), float2Int(temp4));
	  int* result = stud->search(float2Int(temp3), float2Int(temp4));
	  int  matchSize = result[0];
	  fprintf(quOut, "%d\n", matchSize);
	  for (int i = 1; i <= matchSize; i++)
	    fprintf(quOut, "%d\n", result[i]);
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
	  //printf("select * from professor where ProfID = %d", temp1);
	  //fprintf(quOut, "select * from professor where ProfID = %d", temp1);
	  fprintf(quOut, "1\n");
	}
	else if (attr[0]=='S') {
	  // 6. select * from professor where Salary = XXXXXX
	  fscanf(quIn, ", %d\n", &temp1);
	  int* result = prof->search(temp1, temp1);
	  int  matchSize = result[0];
	  fprintf(quOut, "%d\n", matchSize);
	  for (int i = 1; i <= matchSize; i++)
	    fprintf(quOut, "%d\n", result[i]);
	}
	else
	  printf("ERROR: Invalid attribute in professors table\n");
      }
      else if (search[7]=='R') {
	if (attr[0]=='S') {
	  // 7. select * from professor where Salary <= XXXXXX AND Salary >= XXXXXX
	  fscanf(quIn, ", %d, %d\n", &temp1, &temp2);

	  int* result = prof->search(temp1, temp2);
	  int  matchSize = result[0];
	  fprintf(quOut, "%d\n", matchSize);
	  for (int i = 1; i <= matchSize; i++)
	    fprintf(quOut, "%d\n", result[i]);
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
}

int main() {
  BPlusTree* treeStud = new BPlusTree();
  BPlusTree* treeProf = new BPlusTree();
  readStudent(treeStud);
  readProf(treeProf);

  query( treeStud, treeProf);
  
  return 0;
}

#endif
