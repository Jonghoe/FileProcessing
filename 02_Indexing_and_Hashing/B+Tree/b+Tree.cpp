#include "b+Tree.h"
#define NULL 0

BPlusTree::BPlusTree() : rootNode(new TerminalNode()) {}
bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)
bool BPlusTree::readTree() {}  // tngus's part (read the structure from a file)
  
bool BPlusTree::insert(float score, int blckN) {
  Node* insertVal = rootNode.insert(score, blckN);

  // if no overflow
  if (insertVal == NULL)
    return true;
  // if overflow
  else {
    InternalNode* newRootNode = new InterNode();

    newRootNode->storedRecordNumber = 2;
    newRootNode->branchs[0] = rootNode;
    newRootNode->branchs[1] = insertVal;
    newRootNode->scoreDeli[0] = branchs[1]->minVal();

    rootNode = newRootNode;
    
    return true;
  }
}

Node* BPlusTree::searchFirstMatch(float scoreLowerBound) {
  return rootNode->searchFirstMatch(scoreLowerBound);
}

InternalNode::InternalNode() : branchSize(512), scoreSize(511), storedRecordNumber(0) {}

// search for first corresponding score low bound
Node* InternalNode::searchFirstMatch(float scoreLowerBound) {
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
    // 3. checking the last index
    if (scoreDeli[checkSize-1] <= scoreLowerBound)
      return branchs[checkSize]->searchFirstMatch(scoreLowerBound);
  }
}

Node* InternalNode::insert(float score, int blckN) {
  // 1. find where to insert
  int insertIndex = 0;
  //   (1) checking the last index
  if (maxVal() <= score)
    insertIndex = storedRecordNumber - 1;
  //   (2) checking other indice
  else if (!(score < minVal())) {
    for(int i = storedRecordNumber - 2; i > 0; i--)
      if (scores[i] <= score) {
	insertIndex = i;
	break;
      }
  }
  //   (3) checking the first index
  else
    insertIndex = 0;

  // 2. insert
  Node* newVal = branchs[insertIndex]->insert(score, blckN);
  
  // 3. update Deli vals for inserted ones
  scoreDeli[insertIndex-1] = branchs[insertIndex]->minVal();

  
  if (newVal == NULL)
    return NULL;
  // 4. if overflow split
  else if (size < storedRecordNumber + 1) {
    InternalNode* newNode = new InternalNode();
    // not done yet
  }
}  

TerminalNode::TerminalNode() : size(511), storedRecordNumber(0) {}
Node* TerminalNode::searchFirstMatch(float scoreLowerBound) {
  return this;
}

// function to help Terminal::search()
// count how may records correspond to search
int TerminalNode::cntTillUpper(float scoreUpperBound) {
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

// function to help Terminal::search()
// copy matching records' block nums
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

Node* TerminalNode::insert(float score, int blckN) {
  // 1.find where to insert
  int insertIndex = 0;
  //   (1) checking the last index
  if (maxVal() <= score)
    insertIndex = storedRecordNumber + 1;
  //   (2) checking other indice
  else if (!(score < minVal())) {
    for (int i = storedRecordNumber - 1; 0 < i ; i--)
      if ( scores[i-1] <= score && score < scores[i]) {
	insertIndex = i;
	break;
      }
  }
  //   (3) checking the first index
  else
    insertIndex = 0;
  
  // 2. if overflow, split
  bool ifOverflow = false;
  if (size < storedRecordNumber + 1) {
    ifOverflow = true;
    TerminalNode* newNode = new TerminalNode();

    int newRecordNum = storedRecordNumber / 2; // number of record for this block
    for (int i = 0; i < newRecordNum; i++) {
      newNode->scores[i] = scores[i+newRecordNum];
      newNode->blockNum[i] = blockNum[i+newRecordNum];
    }
    
    // rearrange record numbers
    newNode->storedRecordNumber = storedRecordNumber - newRecordNum;
    storedRecordNumber = newRecordNum;

    // switch links
    newNode->nextTerminalNode = nextTerminalNode;
    nextTerminalNode = newNode;
  }

  // 3. insert (if range is in this block)
  if (insertIndex <= storedRecordNumber) {
    for (int i = storedRecordNumber - 1; insertIndex <= i ; i--) {
      scores[i+1] = scores[i];
      blockNum[i+1] = scores[i];
    }
    scores[insertIndex] = score;
    blockNum[insertIndex] = blckN;
    
    storedRecordNumber++;
  }
  else {
    nextTerminalNode->insert(score, blckN);
  }

  // 4. return
  if (ifOverflow)
    return nextTerminalNode;
  else
    return NULL;
}


int main() {
  BPlusTree* tree = new BPlusTree();
  tree->insert(3.0, 1);
  return 0;
}
