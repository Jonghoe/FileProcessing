#include "b+Tree.h"
#include <iostream>
#include <fstream>
#include <bitset>
#ifndef BPLUSTREEIO_CPP
#define BPLUSTREEIO_CPP

using namespace std;

// find kth terminal node
TerminalNode* BPlusTree::findKthTerminal(int k) {
  TerminalNode* findFirstNode = dynamic_cast<TerminalNode*>(searchFirstMatch(0));

  //findFirstNode->print(0);

  for(int i = 1; i < k && findFirstNode!=NULL ; i++)
    findFirstNode = findFirstNode->nextTerminalNode;
  
  if (findFirstNode != NULL)
    findFirstNode->print(0); // this might be changed into printWithBlockNum
  else
    cout << "this is not in range" << endl;
  return nullptr;
}

void BPlusTree::findNode(Node* node, ofstream& ofs){
	if(node->ifTerminal()){
		printNode((TerminalNode*)node,ofs);		
	}
	else {
		InternalNode* in = (InternalNode*)node;
		for(int i = 0; i<in->branchSize; ++i) {
			findNode(in->branchs[i], ofs);
		}
		printNode((InternalNode*)node, ofs);
	}
}

void BPlusTree::printNode(InternalNode * node, ofstream & ofs)
{
	InternalNode* tn = (InternalNode*)node;
	ofs << "I"<<bitset<32>(tn->allocatedBlockNumber) << bitset<32>(tn->storedRecordNumber);
	for (int i = 0; i<tn->branchSize; ++i) {
		ofs << bitset<32>(tn->branchs[i]->allocatedBlockNumber) << bitset<32>(tn->scoreDeli[i]);
	}
}
void BPlusTree::printNode(TerminalNode * node, ofstream & ofs)
{
	TerminalNode* tn = (TerminalNode*)node;
	ofs << "T"<<bitset<32>(tn->allocatedBlockNumber) << bitset<32>(tn->storedRecordNumber);
	for (int i = 0; i<tn->size; ++i) {
		ofs << bitset<32>(tn->scores[i]) << bitset<32>(tn->studID[i]);
	}
}
// store B+Tree into Students_score.idx
bool BPlusTree::storeTree() {
	//((InternalNode*)this->rootNode)->branchs[0]->allocatedBlockNumber
	ofstream ofs("students_score.idx",ios::binary);
	findNode(this->rootNode, ofs);

	return false;
} // tngud's part (store the structure in a file)

// load B+Tree from Students_score.idx
bool BPlusTree::loadTree() {
	char c;
	int size, bln;
	ifstream ifs("students_score.idx", ios::binary);
	vector<TerminalNode*> tns;
	vector<InternalNode*> ins;
	while(!ifs.eof()) {
		ifs>>c>>bln>>size;
		if(c=='T') {
			TerminalNode* nd = new TerminalNode();
			nd->allocatedBlockNumber = bln;
			nd->storedRecordNumber = size;

			for(int i = 0; i<size;++i){
				float score;
				ifs>>score>>bln;
				nd->scores[i] = score;
				nd->studID[i] = bln;
			}
			tns.push_back(nd);
		}
		else {
			InternalNode* nd = new InternalNode();
			nd->allocatedBlockNumber = bln;
			nd->storedRecordNumber = size;
			for(int i = 0; i<size; ++i) {
				float deli;
				ifs>>bln>>deli;
				nd->scoreDeli[i] = deli;
				nd->branchs[i] = (Node*)bln;
			}
			ins.push_back(nd);
		}
	}
	this->rootNode = ins.back();
	ins.pop_back();
	reCreateTree(rootNode,ins,tns);
	return true;
}  // tngud's part (read the structure from a file)

void BPlusTree::reCreateTree(Node* nd,vector<InternalNode*>& ins, vector<TerminalNode*> tns)
{
	if(nd->ifTerminal()) {
		return;
	}
	else {
		InternalNode* node = (InternalNode*)nd;
				
		bool terminal = false;
		int num = (int)node->branchs[0];
		int j = 0;
		for(; j<ins.size()&&!(ins[j]->allocatedBlockNumber==num); ++j)
			;
		if(j<ins.size()) {
			node->branchs[0] = ins[j];
			InternalNode* tmp = ins.back();
			tmp = ins[j];
			ins[j] = tmp;
			ins.pop_back();
		}
		else {
			for(j=0; j<tns.size()&&!(tns[j]->allocatedBlockNumber==num); ++j)
				;
			if(j<tns.size()) {
				node->branchs[0] = tns[j];
				TerminalNode* tmp = tns.back();
				tmp = tns[j];
				tns[j] = tmp;
				tns.pop_back();
			}
			terminal = true;
		}
		for(int i = 1; i<node->storedRecordNumber;++i){
			int num = (int)node->branchs[i];
			if(terminal){
				for(j=0; j<ins.size()&&!(ins[j]->allocatedBlockNumber!=num); ++j)
					;
				node->branchs[i] = ins[j];
				InternalNode* tmp=ins.back();
				tmp = ins[j];
				ins[j] = tmp;
				ins.pop_back();
			}
			else {
				for(j=0; j<tns.size()&&!(tns[j]->allocatedBlockNumber!=num); ++j)
					;
				node->branchs[i] = tns[j];
				TerminalNode* tmp = tns.back();
				tmp = tns[j];
				tns[j] = tmp;
				tns.pop_back();
			}
		}
		for(int i = 0; i<node->branchSize; ++i) {
			reCreateTree(node->branchs[i], ins, tns);
		}
	}
}

void indentPrint(int indent) {
  using namespace std;
  for (int i = 0; i < indent; i++)
    cout << "\t";
}

// these are for print test (block number values are pointer address)
void BPlusTree::print() {rootNode->print(0);}
void InternalNode::print(int indent) {
  indentPrint(indent);
  cout << "<<Internal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber-1; i++) {
    indentPrint(indent);
    cout << "branchs[" << i << "] : " << branchs[i]->allocatedBlockNumber << "\t";
    cout << "scoreDeli[" << i << "] : " << scoreDeli[i] << endl;
  }
  indentPrint(indent);
  cout << "branchs[" << storedRecordNumber-1 << "] : " << branchs[storedRecordNumber-1]->allocatedBlockNumber << endl << endl;

  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "branchs[" << i << "]'s child" << endl;
    branchs[i]->print(indent+1);
  }
}
void TerminalNode::print(int indent) {
  indentPrint(indent);
  cout << "<<Terminal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "scores[" << i << "] : " << scores[i] << "\t";
    cout << "studID[" << i << "] : " << studID[i] << endl;
  }
  cout << endl;
}



// actually print blockNum
void BPlusTree::printWithBlockNum(const HashTable& hashTable) {
  rootNode->printWithBlockNum(0, hashTable);
}

void InternalNode::printWithBlockNum(int indent, const HashTable& hashTable) {
  indentPrint(indent);
  cout << "<<Internal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber-1; i++) {
    indentPrint(indent);
    cout << "branchs[" << i << "] : " << branchs[i]->allocatedBlockNumber << "\t";
    cout << "scoreDeli[" << i << "] : " << scoreDeli[i] << endl;
  }
  indentPrint(indent);
  cout << "branchs[" << storedRecordNumber-1 << "] : " << branchs[storedRecordNumber-1]->allocatedBlockNumber << endl << endl;

  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "branchs[" << i << "]'s child" << endl;
    branchs[i]->printWithBlockNum(indent+1, hashTable);
  }
}

void TerminalNode::printWithBlockNum(int indent, const HashTable& hashTable) {
  indentPrint(indent);
  cout << "<<Terminal Node>> : " << allocatedBlockNumber << endl;
  indentPrint(indent);
  cout << "num of records : " << storedRecordNumber << endl;
  for (int i = 0; i < storedRecordNumber; i++) {
    indentPrint(indent);
    cout << "scores[" << i << "] : " << scores[i] << "\t";
    cout << "blocks[" << i << "] : " << studID[i] << endl;
    //cout << "blocks[" << i << "] : " << hashTable.getBlkNum(blockNum[i]) << endl;
  }
  cout << endl;
}


#endif
