#ifndef BPLUSTREEIO_CPP
#define BPLUSTREEIO_CPP

#include "b+Tree.h"
#include <iostream>
//#include <fstream>
#include <stdio.h>
#include <bitset>
#include <string>

using namespace std;

void indentPrint(int indent) {
  using namespace std;
  for (int i = 0; i < indent; i++)
    cout << "\t";
}

// these are for print test (block number values are pointer address)
void BPlusTree::print() {rootNode->print(0);}
void InternalNode::print(int indent) {
  indentPrint(indent);
  cout << "<<Internal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber-1; i++) {
    indentPrint(indent);
    cout << "branchs[" << i << "] : " << branchs[i]->allocatedBlockNumber << "\t";
    cout << "scoreDeli[" << i << "] : " << scoreDeli[i] << endl;
  }
  indentPrint(indent);
  cout << "branchs[" << storedRecordNumber-1 << "] : " << branchs[storedRecordNumber-1]->allocatedBlockNumber << endl << endl;

  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "branchs[" << i << "]'s child" << endl;
    branchs[i]->print(indent+1);
  }
}
void TerminalNode::print(int indent) {
  indentPrint(indent);
  cout << "<<Terminal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "scores[" << i << "] : " << scores[i] << "\t";
    cout << "studID[" << i << "] : " << studID[i] << endl;
  }
  cout << endl;
  if (nextTerminalNode != NULL)
    cout << "next BlockNumber: " << nextTerminalNode->allocatedBlockNumber << endl;
  else
    cout << "next BlockNumber: None"  << endl;
}


/*
// actually print blockNum
void BPlusTree::printWithBlockNum(const HashTable& hashTable) {
  rootNode->printWithBlockNum(0, hashTable);
}

void InternalNode::printWithBlockNum(int indent, const HashTable& hashTable) {
  indentPrint(indent);
  cout << "<<Internal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber-1; i++) {
    indentPrint(indent);
    cout << "branchs[" << i << "] : " << branchs[i]->allocatedBlockNumber << "\t";
    cout << "scoreDeli[" << i << "] : " << scoreDeli[i] << endl;
  }
  indentPrint(indent);
  cout << "branchs[" << storedRecordNumber-1 << "] : " << branchs[storedRecordNumber-1]->allocatedBlockNumber << endl << endl;

  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "branchs[" << i << "]'s child" << endl;
    branchs[i]->printWithBlockNum(indent+1, hashTable);
  }
}

void TerminalNode::printWithBlockNum(int indent, const HashTable& hashTable) {
  indentPrint(indent);
  cout << "<<Terminal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "scores[" << i << "] : " << scores[i] << "\t";
    cout << "blocks[" << i << "] : " << studID[i] << endl;
    //cout << "blocks[" << i << "] : " << hashTable.getBlkNum(blockNum[i]) << endl;
  }
  cout << endl;
}
*/

void readStudent(BPlusTree* tree) {
  int studNum;
  
  char nameTemp[21];
  int numTemp, profNumTemp;
  //  int scoreTemp, scoreTemp2, length;
  char scoreTemp[10]; int scoreCal, i;
  //  ifstream readStud;
  //  readStud.open("SampleData/student_data.csv");
  FILE *stFP;
  
  stFP = fopen("./SampleData/student_data.csv", "r");
  if(!stFP){
        printf("Error: no student_data.csv found\n");
  }
  fscanf(stFP, "%d\n", &studNum);
  printf("%d\n", studNum);

  //  studNum = 10;

  //  readStud >> studNum;
  while(studNum--) {
    fscanf(stFP, "%[^,],%d,%[^,],%d\n",  nameTemp, &numTemp, scoreTemp, &profNumTemp);
    scoreCal = scoreTemp[0] - '0';
    i = 1;
    for (; scoreTemp[i] != '\0'; i++) {
      if (scoreTemp[i] == '.')
	continue;
      scoreCal = (scoreCal * 10) + (scoreTemp[i] - '0');
    }
    if (i == 1)
      i = 2;
    for (; i < 10; i++)
      scoreCal *= 10;
    
    //printf("%s\t%d\t%d\t%d\n", nameTemp, numTemp, scoreCal, profNumTemp);
    tree->insert(scoreCal, numTemp);    
  }
};


void readProf(BPlusTree* tree) {
  int studNum;
  
  char nameTemp[21];
  int idTemp, salaryTemp;
  FILE *stFP;
  
  stFP = fopen("./SampleData/prof_data.csv", "r");
  if(!stFP){
    printf("Error: no prof_data.csv found\n");
  }
  fscanf(stFP, "%d\n", &studNum);
  printf("%d\n", studNum);

  while(studNum--) {
    fscanf(stFP, "%[^,],%d,%d\n",  nameTemp, &idTemp, &salaryTemp);
    //printf("%s\t%d\t%d\n", nameTemp, idTemp, scoreCal);
    tree->insert(salaryTemp, idTemp);    
  }
};



#endif
