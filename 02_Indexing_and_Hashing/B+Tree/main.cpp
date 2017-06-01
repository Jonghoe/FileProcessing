#include "b+Tree.cpp"
#include "b+TreeSearch.cpp"
#include "b+TreeInsert.cpp"
#include "b+TreeIO.cpp"
//#include "../Index_Hashing/HashTable.cpp"

int main() {
  BPlusTree* tree = new BPlusTree();
  for (int i = 1; i <= 30; i++)
    tree->insert(i, i+1000);
  
  tree->insert(12.5,1000000);
  tree->insert(1,1);
  tree->insert(2, 2);
  
  tree->insert(3, 2);
  //tree->insert(4, 2);
  //tree->insert(5, 2);
  //tree->insert(3.5, 1000);
  

  tree->print();
  //HashTable* htb = new HashTable();
  //tree->printWithBlockNum(*htb);

  //tree->findKthTerminal(11);
  //tree->findKthTerminal(12);
  return 0;
}
