#ifndef BPLUSTREE_H
#define BPLUSTREE_H

class BPlusTree;
class Node {};
class InternalNode;
class TerminalNode;

class BPlusTree{
protected:
  Node* rootNode;
public:
  BPlusTree();

  bool storeTree();
  bool readTree();
  
  bool insert(float score, int blockNum);
  int  exactMatchSearch(float score);
  int* rangeSearch(float score);
};

class InternalNode: public Node {
  friend class BPlusTree;
  
private:
  const int branchSize;
  const int scoreSize;
  int storedRecordNumber;

  Node* branchs[512];
  float scoreDeli[511];
protected:
  InternalNode();
  
  Node* search(float score);
};


class TerminalNode: public Node {
  friend class BPlusTree;
  friend class InternalNode;
  
private:
  const int size;
  int storedRecordNumber;

  float scores[511];
  int   blockNum[511];
  TerminalNode* nextTerminalNode;
protected:
  TerminalNode();
  
  int getBlockNumber(float score);
  int getRangeBlockNumber(float score);
};

#endif
