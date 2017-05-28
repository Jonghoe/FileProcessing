#include "b+Tree.h"

BPlusTree::BPlusTree() : rootNode(new TerminalNode()) {}
bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)
bool BPlusTree::readTree() {}  // tngus's part (read the structure from a file)
  
bool BPlusTree::insert(float score, int blockNum) {
  Node* lookingAt = rootNode;

  while(1) {
    if (lookingAt.ifTermianl()) { // found terminal
      if (!lookingAt.insert(score, blockNum)) { // should do sth if no more space
	
      }
    }
    else { // try finding terminal
      lookingAt = lookingAt.search(score); // this is the first node match
    }
  }
}
int  BPlusTree::exactMatchSearch(float score) {
}
int* BPlusTree::rangeSearch(float score) {
}

InternalNode::InternalNode() : branchSize(512), scoreSize(511) {}
Node* InternalNode::search(float score) {
  if (score < scoreDeli[0])
    return branchs[0];
  else {
    // this is for checking 1..(size-1)
    int checkSize = storedRecordNumber;
    if (checkSize == scoreSize)
      checkSize--;
    
    for (int i = 0; i < checkSize; i++) {
      if (scoreDeli[i] <= score && score < scoreDeli[i+1])
	return branchs[i];
    }
    if (storedRecordNumber == scoreSize && score < scoreDeli[scoreSize-1])
      return branchs[branchSize-1]
      
  }
}

TerminalNode::TerminalNode() : size(511) {}
bool TerminalNode::insert(float score, int blockNum) {
}  
int TerminalNode::getBlockNumber(float score) {
}
int TerminalNode::getRangeBlockNumber(float score) {
}


int main() {
  return 0;
}
