#include "b+Tree.h"
#include <iostream>
//#define NULL 0

BPlusTree::BPlusTree() : rootNode(new TerminalNode()) {}
InternalNode::InternalNode() : branchSize(4), scoreSize(3), storedRecordNumber(0) {}
TerminalNode::TerminalNode() : size(5), storedRecordNumber(0), nextTerminalNode(NULL) {}

//InternalNode::InternalNode() : branchSize(512), scoreSize(511), storedRecordNumber(0) {}
//TerminalNode::TerminalNode() : size(511), storedRecordNumber(0), nextTerminalNode(NULL) {}

bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)
bool BPlusTree::readTree() {}  // tngus's part (read the structure from a file)



// these are for print test
void BPlusTree::print() {rootNode->print();}
void InternalNode::print() {
  using namespace std;

  cout << "Internal Node" << endl;
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber-1; i++) {
    cout << "branchs[" << i << "] : " << branchs[i] << "\t";
    cout << "scoreDeli[" << i << "] : " << scoreDeli[i] << endl;
  }
  cout << "branchs[" << storedRecordNumber-1 << "] : " << branchs[storedRecordNumber-1] << endl << endl;

  for (int i = 0; i < storedRecordNumber; i++) {
    cout << "branchs[" << i << "]'s child" << endl;
    branchs[i]->print();
  }
}
void TerminalNode::print() {
  using namespace std;

  cout << "Terminal Node" << endl;
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber; i++) {
    cout << "scores[" << i << "] : " << scores[i] << "\t";
    cout << "blocks[" << i << "] : " << blockNum[i] << endl;
  }
  cout << endl;
}
