#include "b+Tree.h"
#include <iostream>

#ifndef BPLUSTREEINSERT_CPP
#define BPLUSTREEINSERT_CPP

bool BPlusTree::insert(float score, int blckN) {
  Node* insertVal = rootNode->insert(score, blckN);

  // if no overflow
  if (insertVal == NULL)
    return true;
  // if overflow
  else {
    InternalNode* newRootNode = new InternalNode();

    newRootNode->storedRecordNumber = 2;
    newRootNode->branchs[0] = rootNode;
    newRootNode->branchs[1] = insertVal;
    newRootNode->scoreDeli[0] = newRootNode->branchs[1]->minVal();

    rootNode = newRootNode;
    
    return true;
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
      if (scoreDeli[i] <= score) {
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

  // 4. if no new child node
  if (newVal == NULL)
    return NULL;

  // 5. if new child node, insert new branch and deli
  //    (1) if no overflow
  if (storedRecordNumber + 1 <= branchSize) {

    if (insertABranch(insertIndex+1, newVal))
      return NULL;
    else
      return (Node*) -1;
  }
  //    (2) if overflow
  else {
    if (insertIndex + 1 < storedRecordNumber / 2) { // new record in this block
      InternalNode* nextInternal = overflowSplit(true);
      insertABranch(insertIndex+1, newVal);

      return nextInternal;
    }
    else {// new record in next block
      InternalNode* nextInternal = overflowSplit(false);
      //std::cout << "insertIndex : " << insertIndex << "\tstoredRecordNumber : " << storedRecordNumber << "\t" << insertIndex+1 - storedRecordNumber<< std::endl;
      nextInternal->insertABranch(insertIndex+1 - storedRecordNumber, newVal);

      return nextInternal;
    }
  }
}  

Node* TerminalNode::insert(float score, int blckN) {
  // base case : first insert in the node
  if (storedRecordNumber == 0) {
    scores[0] = score;
    blockNum[0] = blckN;
    
    storedRecordNumber++;
    return NULL;
  }
  
  // 1.find where to insert
  int insertIndex = 0;
  //   (1) checking the last index
  if (maxVal() <= score) 
    insertIndex = storedRecordNumber;
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
  
  //std::cout << "store : " << storedRecordNumber << "\tinsert : " << insertIndex << std::endl;
  // 2. if overflow, split
  bool ifOverflow = false;
  if (size < storedRecordNumber + 1) {
    ifOverflow = true;
    TerminalNode* newNode = new TerminalNode();

    
    int cpyRecordNum; // number of record for next block
    int newRecordNum; // number of record for this block

    if (insertIndex < storedRecordNumber / 2 + 1) {
      cpyRecordNum = (storedRecordNumber+1) / 2;
      newRecordNum = storedRecordNumber - cpyRecordNum;
    }
    else {
      newRecordNum = (storedRecordNumber+1) / 2;
      cpyRecordNum = storedRecordNumber - newRecordNum;
    }
    
    //std::cout << "cpy: " << cpyRecordNum << "\tnew: " << newRecordNum << std::endl;
    for (int i = 0; i < cpyRecordNum; i++) {
      newNode->scores[i] = scores[i+newRecordNum];
      newNode->blockNum[i] = blockNum[i+newRecordNum];
    }
    
    // rearrange record numbers
    newNode->storedRecordNumber = cpyRecordNum;
    storedRecordNumber = newRecordNum;

    // switch links
    newNode->nextTerminalNode = nextTerminalNode;
    nextTerminalNode = newNode;

    // 3. insert (if range is in this block)
    if (insertIndex < storedRecordNumber) {
      for (int i = storedRecordNumber - 1; insertIndex <= i ; i--) {
	scores[i+1] = scores[i];
	blockNum[i+1] = blockNum[i];
      }
      scores[insertIndex] = score;
      blockNum[insertIndex] = blckN;
    
      storedRecordNumber++;
    }
    else {
      //std::cout << "asfdasdfasdf" << nextTerminalNode << std::endl;
      nextTerminalNode->insert(score, blckN);
    }

    return nextTerminalNode;
  }
  else { // if no split, insert
    for (int i = storedRecordNumber - 1; insertIndex <= i ; i--) {
      scores[i+1] = scores[i];
      blockNum[i+1] = blockNum[i];
    }
    scores[insertIndex] = score;
    blockNum[insertIndex] = blckN;
    
    storedRecordNumber++;

    return NULL;
  }
}




// function to help internalNode::insert()
// insert a branch into thhe internalNode
bool InternalNode::insertABranch(int insertIndex, Node* newVal) { // insertIndex must be > 0
  // if the insertion cause an overflow
  if (storedRecordNumber + 1 > branchSize) {
    return false;
  }

  //std::cout << "store : " << storedRecordNumber << "\tinsertIndex : " << insertIndex << std::endl;
  if (storedRecordNumber != insertIndex) {
    for (int i = storedRecordNumber; i >= insertIndex; i--) {
      branchs[i+1] = branchs[i];
      scoreDeli[i] = scoreDeli[i-1];
    }
  }

  branchs[insertIndex] = newVal;
  scoreDeli[insertIndex-1] = newVal->minVal();

  storedRecordNumber++;
  
  return true;
}

// function to help internalNode::insert()
// split a node in two
InternalNode* InternalNode::overflowSplit(bool ifNewAtThis) {
  // 1. calculate numbers in block
  int newStoreNum;  // new number of this block
  int cpyRecordNum; // new number of next block

  if (ifNewAtThis) { // if the new node will be inserted in this node
    newStoreNum  = (storedRecordNumber + 1) / 2;
    cpyRecordNum = (storedRecordNumber + 1) - newStoreNum;
  }
  else { // if the new node will be inserted in next node
    cpyRecordNum = (storedRecordNumber + 1) / 2;
    newStoreNum  = (storedRecordNumber + 1) - cpyRecordNum;
  }

  //  std::cout << ifNewAtThis << "\tnew : " << newStoreNum << "\tcpy : " << cpyRecordNum << std::endl;

  // 2. copy to the new node
  InternalNode* newInternal = new InternalNode();
  for (int i = 0; i < cpyRecordNum-2; i++) {
    //std::cout << "i : " << i << std::endl;
    newInternal->branchs[i] = branchs[newStoreNum+i];
    newInternal->scoreDeli[i] = scoreDeli[newStoreNum+i];
  }
  newInternal->branchs[cpyRecordNum-2] = branchs[storedRecordNumber - 1];

  // 3. set StoreNum vals
  storedRecordNumber = newStoreNum;
  newInternal->storedRecordNumber = cpyRecordNum - 1; // will insert and be ++

  return newInternal;
}

#endif
