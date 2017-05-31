#include "b+Tree.cpp"
#include "b+TreeSearch.cpp"
#include "b+TreeInsert.cpp"

int main() {
  BPlusTree* tree = new BPlusTree();
  //  for (int i = 1; i <= 10; i++)
    tree->insert(3.0, 1);
    tree->print();
    tree->insert(3.0, 2);
    tree->print();
    tree->insert(4.0, 3);
    tree->print();
    tree->insert(1.0, 4);
    tree->print();
    tree->insert(2.0, 5);
    tree->print();
  return 0;
}
