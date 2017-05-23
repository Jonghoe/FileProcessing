#include "b+Tree.h"

BPlusTree::BPlusTree() {}
bool BPlusTree::storeTree() {
}
bool BPlusTree::readTree() {
}
  
bool BPlusTree::insert(float score, int blockNum) {
}
int  BPlusTree::exactMatchSearch(float score) {
}
int* BPlusTree::rangeSearch(float score) {
}

InternalNode::InternalNode() : branchSize(512), scoreSize(511) {}
Node* InternalNode::search(float score) {
}

TerminalNode::TerminalNode() : size(511){}
  
int TerminalNode::getBlockNumber(float score) {
}
int TerminalNode::getRangeBlockNumber(float score) {
}


int main() {
  return 0;
}
