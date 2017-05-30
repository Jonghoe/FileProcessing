#include "b+Tree.h"

BPlusTree::BPlusTree() : rootNode(new TerminalNode()) {}
bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)
bool BPlusTree::readTree() {}  // tngus's part (read the structure from a file)
  
bool BPlusTree::insert(float score, int blockNum) {
  Node* lookingAt = rootNode;

  while(1) {
    if (lookingAt.ifTermianl()) { // found terminal
      //if (score ) // where?
      //if (!lookingAt.insert(score, blockNum)) { // should do sth if no more space	
      //}
    }
    else { // try finding terminal
      lookingAt = lookingAt.search(score); // this is the first node match
    }
  }
}

InternalNode::InternalNode() : branchSize(512), scoreSize(511) {}

// search for forst corresponding score low bound
int* InternalNode::search(float scoreLowerBound, float scoreUpperBound) {
  if (scoreLowBound < scoreDeli[0])
    return branchs[0].search(scoreLowerBound, scoreUpperBound);
  else {
    // this is for checking 1..(size-1)
    int checkSize = storedRecordNumber;
    if (checkSize == scoreSize)
      checkSize--;
    
    for (int i = 0; i < checkSize; i++) {
      if (scoreDeli[i] <= scoreLowBound && scoreLowBound < scoreDeli[i+1])
	return branchs[i].search(scoreLowerBound, scoreUpperBound);
    }
    if (storedRecordNumber == scoreSize && scoreLowBound < scoreDeli[scoreSize-1])
      return branchs[branchSize-1].search(scoreLowerBound, scoreUpperBound)
  }
}

TerminalNode::TerminalNode() : size(511) {}
int* TerminalNode::search(float scoreLowerBound, float scoreUpperBound) {
}
bool TerminalNode::insert(float score, int blockNum) {
}  


int main() {
  return 0;
}
