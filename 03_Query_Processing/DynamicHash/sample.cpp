#include"b+Tree.h"
//#include "../Index_Hashing/HashTable.cpp"

int main()
{
	BPlusTree* tree = new BPlusTree();
	//readStudent(tree);
	readProf(tree);
	tree->print();
	/*
	int* result = tree->search(120252000, 120522000);
	printf("%d\n\n", result[0]);
	int matchSize = result[0];
	for (int i = 1; i <= matchSize; i++) {
	printf("%d\t", result[i]);
	if (i%10 == 0)
	printf("\n");
	}*/

	return 0;
}
