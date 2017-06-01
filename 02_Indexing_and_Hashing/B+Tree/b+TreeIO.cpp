#include "b+Tree.h"
#include <iostream>

#ifndef BPLUSTREEIO_CPP
#define BPLUSTREEIO_CPP

using namespace std;

TerminalNode* BPlusTree::findKthTerminal(int k) {
  TerminalNode* findFirstNode = dynamic_cast<TerminalNode*>(searchFirstMatch(0));

  //findFirstNode->print(0);

  for(int i = 1; i < k && findFirstNode!=NULL ; i++)
    findFirstNode = findFirstNode->nextTerminalNode;

  if (findFirstNode != NULL)
    findFirstNode->print(0);
  else
    cout << "this is not in range" << endl;
}

#endif
