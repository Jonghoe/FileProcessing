
#ifndef BPLUSTREE_H
#define BPLUSTREE_H
//#include "../Index_Hashing//HashTable.h"
#include <fstream>
using namespace std;

class BPlusTree;
class Node;
class InternalNode;
class TerminalNode;

class BPlusTree {
protected:
  Node* rootNode;
  /*
  void findNode(Node* node, ofstream& ofs);  
  void printNode(InternalNode* node, ofstream& ofs);
  void printNode(TerminalNode* node, ofstream& ofs);
  void reCreateTree(Node*,vector<InternalNode*>& ins, vector<TerminalNode*> tns);
  */
public:
  BPlusTree();

  bool storeTree();
  bool loadTree();

  bool deleteTree();
  
  bool insert(int score, int idIn);
  //  TerminalNode* findKthTerminal(int k);
  // return first match terminal node
  Node* searchFirstMatch(int scoreLowerBound);
  int*  search(int scoreLowerBound, int scoreUpperBound);
  void  print();
  //  void printWithBlockNum(const HashTable& hashTable);
};

class Node {
 public:
  Node();
  int   allocatedBlockNumber;
  static  int   blockNumCounter;
  virtual bool  ifTerminal() = 0;
  virtual bool  deleteTree() = 0;
  virtual Node* searchFirstMatch(int scoreLowerBound) = 0;
  virtual Node* insert(int score, int idIn) = 0;
  virtual int   minVal() = 0;
  virtual int   maxVal() = 0;
  virtual void  print(int)  = 0;
  //  virtual void  printWithBlockNum(int indent, const HashTable& hashTable) = 0;
};


class InternalNode: public Node {
  friend class BPlusTree;
  
private:
  const int branchSize; //512
  const int scoreSize;  //511

  // contents of the block
  int   storedRecordNumber;
  Node* branchs[512];
  int   scoreDeli[511];

protected:
  //const int allocatedBlockNumber;
  
  InternalNode();
  bool  ifTerminal() {return false;}
  int   minVal() {return scoreDeli[0];}
  int   maxVal() {return scoreDeli[storedRecordNumber-2];}

  bool deleteTree();

  // functions to help internalNode::insert()
  bool insertABranch(int insertIndex, Node* newVal);
  InternalNode* overflowSplit(bool ifNewAtThis);
  
  // return first match terminal node
  Node* searchFirstMatch(int scoreLowerBound);
  Node* insert(int score, int idIn);
  
public:
  void print(int indent);
  //  void printWithBlockNum(int indent, const HashTable& hashTable);
};


class TerminalNode: public Node {
  friend class BPlusTree;
  friend class InternalNode;
  
private:
  const int size;
  
  // contents of the block
  int storedRecordNumber;
  TerminalNode* nextTerminalNode;
  int   scores[511];
  int   studID [511]; // this should be changed into studentNumber

  // functions to help Terminal::search()
  int  cntTillUpper(int scoreUpperBound);
  bool cpyMatchRecords(int* blockNums, int startIndex, int cpyLeft);
  
protected:
  //const int allocatedBlockNumber;
  
  TerminalNode();
  bool  ifTerminal() {return true;}
  int   minVal() {return scores[0];}
  int   maxVal() {return scores[storedRecordNumber-1];}

  bool deleteTree();
  
  Node* searchFirstMatch(int scoreLowerBound);
  Node* insert(int score, int idIn);
  int*  search(int scoreLowerBound, int scoreUpperBound);

public:
  void print(int indent);
  //  void printWithBlockNum(int indent, const HashTable& hashTable);
};

#endif
