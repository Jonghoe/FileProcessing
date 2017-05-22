#ifndef BPLUSTREE_H
#define BPLUSTREE_H

class BPlusTree{
 protected:
  InternalNode* rootNode;
 public:
  BPlusTree();
  bool insert(unsigned int studentID, unsigned int blockNum);
};

class InternalNode {
 private:
  const int size;
  
 public:
};

class TerminalNode {
};

#endif
