#include "b+Tree.h"
#include <iostream>
//#define NULL 0

BPlusTree::BPlusTree() : rootNode(new TerminalNode()) {}
InternalNode::InternalNode() : branchSize(6), scoreSize(5), storedRecordNumber(0) {}
TerminalNode::TerminalNode() : size(5), storedRecordNumber(0), nextTerminalNode(NULL) {}

//InternalNode::InternalNode() : branchSize(512), scoreSize(511), storedRecordNumber(0) {}
//TerminalNode::TerminalNode() : size(511), storedRecordNumber(0), nextTerminalNode(NULL) {}

bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)
bool BPlusTree::readTree() {}  // tngus's part (read the structure from a file)


void indentPrint(int indent) {
  using namespace std;
  for (int i = 0; i < indent; i++)
    cout << "\t";
}

// these are for print test
void BPlusTree::print() {rootNode->print(0);}
void InternalNode::print(int indent) {
  using namespace std;

  indentPrint(indent);
  cout << "<<Internal Node>>" << endl;
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
  using namespace std;

  indentPrint(indent);
  cout << "Terminal Node" << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "scores[" << i << "] : " << scores[i] << "\t";
    cout << "blocks[" << i << "] : " << blockNum[i] << endl;
  }
  cout << endl;
}
