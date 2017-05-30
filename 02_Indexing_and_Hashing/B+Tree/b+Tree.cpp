#include "b+Tree.h"

BPlusTree::BPlusTree() : rootNode(new TerminalNode()) {}
bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)
bool BPlusTree::readTree() {}  // tngus's part (read the structure from a file)
  
bool BPlusTree::insert(float score, int blockNum) {
  /*
  int* lookingAt = rootNode;

  while(1) {
    if (lookingAt->ifTerminal()) { // found terminal
      //if (score ) // where?
      //if (!lookingAt.insert(score, blockNum)) { // should do sth if no more space	
      //}
    }
    else { // try finding terminal
      lookingAt = lookingAt->search(score, score); // this is the first node match
    }
    }*/
}
Node* BPlusTree::searchFirstMatch(float scoreLowerBound) {
  return rootNode->searchFirstMatch(scoreLowerBound);
}

InternalNode::InternalNode() : branchSize(512), scoreSize(511) {}

// search for first corresponding score low bound
Node* InternalNode::searchFirstMatch(float scoreLowerBound) {
  // 1. checking the last index which is branch[0]
  if (scoreLowerBound < scoreDeli[0])
    return branchs[0]->searchFirstMatch(scoreLowerBound);
  else {
    // 2. this is for checking 1..(size-1)
    int checkSize = storedRecordNumber - 1;
    
    for (int i = 1; i < checkSize; i++) {
      if (scoreDeli[i-1] <= scoreLowerBound && scoreLowerBound < scoreDeli[i])
	return branchs[i]->searchFirstMatch(scoreLowerBound);
    }
    // 3. checking the last index
    if (scoreDeli[checkSize-1] <= scoreLowerBound)
      return branchs[checkSize]->searchFirstMatch(scoreLowerBound); // branch[511]
  }
}

TerminalNode::TerminalNode() : size(511) {}
Node* TerminalNode::searchFirstMatch(float scoreLowerBound) {
  return this;
}

// count how may records correspond to search
int  TerminalNode::cntTillUpper(float scoreUpperBound) {
  // if upperbound is bigger than max
  if (maxVal() < scoreUpperBound) {
    int nextNums = 0;
    if (nextTerminalNode != NULL)
      nexNums = nextTerminalNode->cntTillUpper(scoreUpperBound);
    else
      return storedRecordNumber + nextNums;
  }
  // other cases
  else {
    for (int i = 0; i < storedRecordNumber; i++)
      if (scoreUpperBound < scores[i])
	return i;
  }
}

bool TerminalNode::cpyMatchRcords(int* blockNums, int startIndex) {
}

int* TerminalNode::search(float scoreLowerBound, float scoreUpperBound) {
  // if first match node is not in this node but next one
  if (maxVal() < scoreLoweBound) {
    if (nextTerminalNode != NULL)
      return nextTerminalNode->search(scoreLowerBound, scoreUpperBound);
    else
      return NULL
  }
  
  // 1. find where lower bound starts
  int matchSize = 0;
  int startIndex = 0;
  for (int i = 0; i < storedRecordNumber; i++) {
    if (!(scoreLowerBound <= scores[i])) {
      startIndex = i - 1;
      break;
    }
  }

  // 2. find & count where upper bound ends
  

  // 3. copy matching records' block numbers
  int* blockNums = new Int[matchSize];
}
bool TerminalNode::insert(float score, int blockNum) {
}  


int main() {
  return 0;
}
