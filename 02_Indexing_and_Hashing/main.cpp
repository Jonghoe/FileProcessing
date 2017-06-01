// This is the execution file
//  that integrates db, hash, and b+tree
#include <iostream>

#include "B+Tree/b+Tree.cpp"
#include "B+Tree/b+TreeSearch.cpp"
#include "B+Tree/b+TreeInsert.cpp"
#include "B+Tree/b+TreeIO.cpp"

using namespace std;

void printMenu() {
  cout << "<<Select one of below>>" << endl;
  cout << "[1] Insert records from a file" << endl;
  cout << "[2] Insert a record" << endl;
  cout << "[3] Load from 'Students.DB', 'Students.hash', and 'Students_score.idx'" << endl;
  cout << "[4] Print DB" << endl;
  cout << "[5] Print Hash" << endl;
  cout << "[6] Print kth leaf node" << endl;
  cout << "[7] Exit" << endl;

  cout << "Insert a number : ";
}

int main() {
  int selected = 0, k;
  BPlusTree* tree = new BPlusTree();

  while(1) {
    printMenu();
    
    cin >> selected;
    switch(selected) {
    case 1 : ;
      break;
    case 2 : ;
      break;
    case 3 : ;
      break;
    case 4 : ;
      break;
    case 5 : ;
      break;
    case 6 :
      cout << "Enter number k : ";
      cin >> k;
      tree->findKthTerminal(k)->print(0);
      break;
    case 7 :
      cout << endl << endl << "Bye my friend" << endl << endl;
      return 0;
      break;
    default :
      cout << "Wrong Number is selected." << endl;
      cout << "Please insert a number between 1 and 6." << endl << endl;
      break;
    }

    cout << endl << endl;
  }
  
  return 0;
}
