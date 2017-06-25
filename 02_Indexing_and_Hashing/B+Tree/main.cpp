#include "b+Tree.cpp"
#include "b+TreeSearch.cpp"
#include "b+TreeInsert.cpp"

#include "b+TreeIO2.cpp"

//#include "../Index_Hashing/HashTable.cpp"

int main() {
  BPlusTree* tree = new BPlusTree();
  readStudent(tree);
  tree->print();
  /*
  BPlusTree* tree = new BPlusTree();
  for (int i = 1; i <= 100000000; i++)
    tree->insert(i, i+1000);
  
  tree->insert(12.5,1000000);
  tree->insert(1,1);
  tree->insert(2, 2);
  
  tree->insert(3, 2);
  //tree->insert(4, 2);
  //tree->insert(5, 2);
  //tree->insert(3.5, 1000);
  

  tree->print();
  */
  return 0;
}
