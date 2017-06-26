#pragma once
#include"FileIOManager.h"

typedef int flag;
const int NESTEDBLOCKLOOPJOIN = 1000;

void PrintJoinData(FILE* fp, const Student& stu, const Professor pro);
void JoinProcessing(FILE* fp, flag f);

vector<int> changeID2BlkNum(int* ids, HashTable& tlb);
void FindRecord(vector<int>& blknums, int min, int max, FILE* fp, int flag);
int query(BPlusTree& stu, BPlusTree& pro, HashTable& stuH, HashTable& proH);