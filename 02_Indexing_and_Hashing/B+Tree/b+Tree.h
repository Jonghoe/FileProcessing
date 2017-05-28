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
  const int branchSize; //512
  const int scoreSize;  //511
  int storedRecordNumber;

  Node* branchs[512];
  float scoreDeli[511];
  InternalNode* parent;
protected:
  InternalNode();
  bool  ifTerminal() {return false;}
  
  Node* search(float score);
};


class TerminalNode: public Node {
  friend class BPlusTree;
  friend class InternalNode;
  
private:
  const int size;
  int storedRecordNumber;

  float scores[511];
  int   blockNum[511]; // this should be changed into pointer
  InternalNode* parent;
  TerminalNode* nextTerminalNode;
protected:
  TerminalNode();
  bool  ifTerminal() {return true;}

  bool insert(float score, int blockNum);
  int getBlockNumber(float score);
  int getRangeBlockNumber(float score);
};

#endif
