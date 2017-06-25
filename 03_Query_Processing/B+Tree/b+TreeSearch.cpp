#include "b+Tree.h"

#ifndef BPLUSTREESEARCH_CPP
#define BPLUSTREESEARCH_CPP


Node* BPlusTree::searchFirstMatch(int scoreLowerBound) {
  return rootNode->searchFirstMatch(scoreLowerBound);
}

// search for first corresponding score low bound
Node* InternalNode::searchFirstMatch(int scoreLowerBound) {
  // 1. checking the first index which is branch[0]
  if (scoreLowerBound < scoreDeli[0])
    return branchs[0]->searchFirstMatch(scoreLowerBound);
  else {
    // 2. this is for checking 1..(size-1)
    int checkSize = storedRecordNumber - 1;
    
    for (int i = 1; i < checkSize; i++) {
      if (scoreDeli[i-1] <= scoreLowerBound && scoreLowerBound < scoreDeli[i])
	return branchs[i]->searchFirstMatch(scoreLowerBound);
    }
    // 3. checking the last index (always match if no match above)
    //if (scoreDeli[checkSize-1] <= scoreLowerBound)
    return branchs[checkSize]->searchFirstMatch(scoreLowerBound);
  }
}

Node* TerminalNode::searchFirstMatch(int scoreLowerBound) {
  return this;
}

// function to help Terminal::search()
// count how may records correspond to search
int TerminalNode::cntTillUpper(int scoreUpperBound) {
  // if upperbound is bigger than max
  if (maxVal() < scoreUpperBound) {
    int nextNums = 0;
    if (nextTerminalNode != NULL) // get nextNums
      nextNums = nextTerminalNode->cntTillUpper(scoreUpperBound);
    
    return storedRecordNumber + nextNums;
  }
  // other cases
  else {
    for (int i = 0; i < storedRecordNumber; i++)
      if (scoreUpperBound < scores[i])
	return i;
  }
}


// function to help Terminal::search()
// copy matching records' block nums
bool TerminalNode::cpyMatchRecords(int* blockNums, int startIndex, int cpyLeft) {
  int i = 0;
  while(cpyLeft > 0) {
    // done copying all of datas in this block
    if (i >= storedRecordNumber)
      break;

    studID[startIndex] = studID[i++];
    
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

int* TerminalNode::search(int scoreLowerBound, int scoreUpperBound) {
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
    studID[cpyedNum++] = studID[i]; // copy matching records in this block
  cpyMatchRecords( blockNums, cpyedNum, matchSize - cpyedNum); // copy in others

  // 4. return
  return blockNums;
}


#endif
