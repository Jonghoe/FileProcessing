#include "b+Tree.h"
#define NULL 0

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
      nextNums = nextTerminalNode->cntTillUpper(scoreUpperBound);
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

bool TerminalNode::cpyMatchRecords(int* blockNums, int startIndex, int cpyLeft) {
  int i = 0;
  while(cpyLeft > 0) {
    // done copying all of datas in this block
    if (i >= storedRecordNumber)
      break;

    blockNums[startIndex] = blockNum[i++];
    
    startIndex++;
    cpyLeft--;
  }
  
  if (cpyLeft > 0) // more left to copy to next node
    return nextTerminalNode->cpyMatchRecords(blockNums, startIndex, cpyLeft);
  else if (cpyLeft == 0) // finish copying for the search
    return true;
  else
    return false;
}

int* TerminalNode::search(float scoreLowerBound, float scoreUpperBound) {
  // if first match node is not in this node but next one
  if (maxVal() < scoreLowerBound) {
    if (nextTerminalNode != NULL)
      return nextTerminalNode->search(scoreLowerBound, scoreUpperBound);
    else
      return NULL;
  }
  
  // 1. find where lower bound starts
  int startIndex = 0;
  for (int i = 0; i < storedRecordNumber; i++) {
    if (!(scoreLowerBound <= scores[i])) {
      startIndex = i - 1;
      break;
    }
  }

  // 2. find & count where upper bound ends
  int matchSize = cntTillUpper(scoreUpperBound) - (startIndex + 1);

  // 3. copy matching records' block numbers
  int* blockNums = new int[matchSize];
  int  endIndex = startIndex + matchSize; // the last index to copy in this block
  if (endIndex < storedRecordNumber)
    endIndex = storedRecordNumber;

  int cpyedNum = 0;
  for(int i = startIndex; i < endIndex; i++)
    blockNums[cpyedNum++] = blockNum[i]; // copy matching records in this block
  cpyMatchRecords( blockNums, cpyedNum, matchSize - cpyedNum); // copy in others

  // 4. return
  return blockNums;
}

bool TerminalNode::insert(float score, int blockNum) {
}  


int main() {
  return 0;
}
