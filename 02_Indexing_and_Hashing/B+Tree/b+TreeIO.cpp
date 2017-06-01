#include "b+Tree.h"
#include <iostream>

#ifndef BPLUSTREEIO_CPP
#define BPLUSTREEIO_CPP

TerminalNode* BPlusTree::findKthTerminal(int k) {
  TerminalNode* findFirstNode = dynamic_cast<TerminalNode*>(searchFirstMatch(0));

  findFirstNode->print(0);
}

#endif
