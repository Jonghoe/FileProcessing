#ifndef BPLUSTREE_H
#define BPLUSTREE_H

class BPlusTree;
class Node;
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
  // return first match terminal node
  Node* searchFirstMatch(float scoreLowerBound);
};

class Node {
 public:
  virtual bool  ifTerminal() = 0;
  virtual Node* searchFirstMatch(float scoreLowerBound) = 0;
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
  // return first match terminal node
  Node* searchFirstMatch(float scoreLowerBound);
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

  // functions to help Terminal::search()
  int  cntTillUpper(float scoreUpperBound);
  bool cpyMatchRcords(int* blockNums, int startIndex);
protected:
  TerminalNode();
  bool  ifTerminal() {return true;}
  int   minVal() {return scores[0];}
  int   maxVal() {return scores[storedRecordNumber-1];}

  Node* searchFirstMatch(float scoreLowerBound);
  int*  search(float scoreLowerBound, float scoreUpperBound);
  bool  insert(float score, int blockNum);
};

#endif
