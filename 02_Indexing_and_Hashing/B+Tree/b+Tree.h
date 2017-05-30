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
  // return corresponding record's block numbers
  int* search(float scoreLowBound, float scoreUpperBound);
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
  // search for forst corresponding score low bound
  int* search(float scoreLowBound, float scoreUpperBound);
};


class TerminalNode: public Node {
  friend class BPlusTree;
  friend class InternalNode;
  
private:
  const int size;

  // contents of the block
  int storedRecordNumber;
  TerminalNode* nextTerminalNode;
  float scores[511];
  int   blockNum[511]; // this should be changed into pointer

protected:
  TerminalNode();nb
  bool  ifTerminal() {return true;}
  int   minVal() {return scores[0];}
  int   maxVal() {return socres[storedRecordNumber-1];}

  int* search(float scoreLowBound, float scoreUpperBound);
  bool insert(float score, int blockNum);
};

#endif
