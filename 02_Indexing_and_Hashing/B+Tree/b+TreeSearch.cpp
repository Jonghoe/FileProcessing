#include "b+Tree.h"

#ifndef BPLUSTREESEARCH_CPP
#define BPLUSTREESEARCH_CPP

int* BPlusTree::search(int scoreLowerBound, int scoreUpperBound) {
  TerminalNode* firstmatch = (TerminalNode *) searchFirstMatch(scoreLowerBound);
  printf("<<First match TerminalNode>> : %d, %d\n", firstmatch->allocatedBlockNumber, firstmatch->minVal());
  return firstmatch->search(scoreLowerBound, scoreUpperBound);
}

Node* BPlusTree::searchFirstMatch(int scoreLowerBound) {
  return rootNode->searchFirstMatch(scoreLowerBound);
}

// search for first corresponding score low bound
Node* InternalNode::searchFirstMatch(int scoreLowerBound) {
  // 1. checking the first index which is branch[0]
  if (scoreLowerBound <= scoreDeli[0])
    return branchs[0]->searchFirstMatch(scoreLowerBound);
  else {
    // 2. this is for checking 1..(size-1)
    int checkSize = storedRecordNumber - 1;
    
    for (int i = 1; i < checkSize; i++) {
      if (scoreDeli[i-1] <= scoreLowerBound && scoreLowerBound <= scoreDeli[i])
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
  if (maxVal() <= scoreUpperBound) {
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

    blockNums[startIndex+1] = studID[i++];
    
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
    printf("The first node match is not in terminalNode %d\n", allocatedBlockNumber);
    if (nextTerminalNode != NULL)
      return nextTerminalNode->search(scoreLowerBound, scoreUpperBound);
    else
      return NULL;
  }
  
  // 1. find where lower bound starts
  int startIndex = 0;
  for (int i = 0; i < storedRecordNumber; i++) {
    if (scoreLowerBound <= scores[i]) {
      startIndex = i;
      break;
    }
  }
  printf("The start index match is %d\n", startIndex);

  // 2. find & count where upper bound ends
  int matchSize, nextSize = 0;
  if (nextTerminalNode != NULL)
    nextSize = nextTerminalNode->cntTillUpper(scoreUpperBound);
  if (nextSize == 0) { // when range is within the block
    printf("The last match node is in this terminalNode %d\n", allocatedBlockNumber);
    for (int i = startIndex; i < storedRecordNumber; i++)
      if (scoreUpperBound < scores[i]) {
	matchSize = i - startIndex;
	break;
      }
    printf("The matchSize is %d\n", matchSize);
    // 3. copy matching records' block numbers
    int* blockNums = new int[matchSize+1];
    blockNums[0]   = matchSize;
    
    for (int i = 0; i < matchSize; i++)
      blockNums[i+1] = studID[startIndex+i];
    
    // 4. return
    return blockNums;
  }
  else {
    matchSize = nextSize + (storedRecordNumber - startIndex);

    // 3. copy matching records' block numbers
    // the first index indicate the size of block except the first size memory space
    int* blockNums = new int[matchSize+1];
    blockNums[0]   = matchSize;

    int cpyedNum = 0;
    for(int i = startIndex; i < storedRecordNumber; i++)
      blockNums[(cpyedNum++)+1] = studID[i]; // copy matching records in this block
    nextTerminalNode->cpyMatchRecords( blockNums, cpyedNum, matchSize - cpyedNum); // copy in others

    // 4. return
    return blockNums;
  }
}

#endif
