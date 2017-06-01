#include "b+Tree.h"
#include <iostream>

#ifndef BPLUSTREEIO_CPP
#define BPLUSTREEIO_CPP

using namespace std;

// find kth terminal node
TerminalNode* BPlusTree::findKthTerminal(int k) {
  TerminalNode* findFirstNode = dynamic_cast<TerminalNode*>(searchFirstMatch(0));

  //findFirstNode->print(0);

  for(int i = 1; i < k && findFirstNode!=NULL ; i++)
    findFirstNode = findFirstNode->nextTerminalNode;

  if (findFirstNode != NULL)
    findFirstNode->print(0);
  else
    cout << "this is not in range" << endl;
}

// store B+Tree into Students_score.idx
bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)

// load B+Tree from Students_score.idx
bool BPlusTree::loadTree() {}  // tngus's part (read the structure from a file)



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
    cout << "branchs[" << i << "] : " << branchs[i] << "\t";
    cout << "scoreDeli[" << i << "] : " << scoreDeli[i] << endl;
  }
  indentPrint(indent);
  cout << "branchs[" << storedRecordNumber-1 << "] : " << branchs[storedRecordNumber-1] << endl << endl;

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
    cout << "blocks[" << i << "] : " << blockNum[i] << endl;
  }
  cout << endl;
}



// actually print blockNum
void BPlusTree::printWithBlockNum(int* hashTable) {
  rootNode->printWithBlockNum(0, hashTable);
}

void InternalNode::printWithBlockNum(int indent, int* hashTable) {
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

void TerminalNode::printWithBlockNum(int indent, int* hashTable) {
  indentPrint(indent);
  cout << "<<Terminal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "scores[" << i << "] : " << scores[i] << "\t";
    cout << "blocks[" << i << "] : " << blockNum[i] << endl; // this part should be changed into blockNums
  }
  cout << endl;
}


#endif
