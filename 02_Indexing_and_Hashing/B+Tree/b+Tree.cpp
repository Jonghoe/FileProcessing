#include "b+Tree.h"
#include <iostream>
//#define NULL 0

BPlusTree::BPlusTree() : rootNode(new TerminalNode()) {}  
InternalNode::InternalNode() : branchSize(512), scoreSize(511), storedRecordNumber(0) {}
TerminalNode::TerminalNode() : size(511), storedRecordNumber(0), nextTerminalNode(NULL) {}

bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)
bool BPlusTree::readTree() {}  // tngus's part (read the structure from a file)

void BPlusTree::print() {rootNode->print();}
void InternalNode::print() {
}
void TerminalNode::print() {
  using namespace std;
  
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber; i++) {
    cout << "scores[" << i << "] : " << scores[i] << "\t";
    cout << "blocks[" << i << "] : " << blockNum[i] << endl;
  }
  cout << endl;
}
