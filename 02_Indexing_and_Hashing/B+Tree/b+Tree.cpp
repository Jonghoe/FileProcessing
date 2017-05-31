#include "b+Tree.h"
#define NULL 0

BPlusTree::BPlusTree() : rootNode(new TerminalNode()) {}  
InternalNode::InternalNode() : branchSize(512), scoreSize(511), storedRecordNumber(0) {}
TerminalNode::TerminalNode() : size(511), storedRecordNumber(0), nextTerminalNode(NULL) {}

bool BPlusTree::storeTree() {} // tngud's part (store the structure in a file)
bool BPlusTree::readTree() {}  // tngus's part (read the structure from a file)


