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
    return findFirstNode; // this might be changed into printWithBlockNum
  else {
	  return nullptr;
  }
}

void BPlusTree::findNode(Node* node, ofstream& ofs){
	if(node->ifTerminal()){
		printNode((TerminalNode*)node,ofs);		
	}
	else {
		InternalNode* in = (InternalNode*)node;
		for(int i = 0; i<in->storedRecordNumber; ++i) {
			findNode(in->branchs[i], ofs);
		}
		printNode((InternalNode*)node, ofs);
	}
}

void BPlusTree::printNode(InternalNode * node, ofstream & ofs)
{
	InternalNode* tn = (InternalNode*)node;
	ofs.write((char*)"I", 1);
	ofs.write((char*)&tn->allocatedBlockNumber, 4);
	ofs.write((char*)&tn->storedRecordNumber, 4);
	for (int i = 0; i<tn->storedRecordNumber; ++i) {
		ofs.write((char*)&tn->branchs[i]->allocatedBlockNumber, 4);
		ofs.write((char*)&tn->scoreDeli[i], 4);
	}
}
void BPlusTree::printNode(TerminalNode * node, ofstream & ofs)
{
	TerminalNode* tn = (TerminalNode*)node;
	ofs.write((char*)"T", 1);
	ofs.write((char*)&tn->allocatedBlockNumber, 4);
	ofs.write((char*)&tn->storedRecordNumber, 4);
	for (int i = 0; i<tn->storedRecordNumber; ++i) {
		ofs.write((char*)&tn->scores[i], 4);
		ofs.write((char*)&tn->studID[i], 4);
	}
	int A = 0;
	if(tn->nextTerminalNode==NULL)
		ofs.write((char*)&A, 4);
	else
		ofs.write((char*)&tn->nextTerminalNode->allocatedBlockNumber, 4);
}
// store B+Tree into Students_score.idx
bool BPlusTree::storeTree() {
	//((InternalNode*)this->rootNode)->branchs[0]->allocatedBlockNumber
	ofstream ofs("students_score.idx",ios::binary);
	findNode(this->rootNode, ofs);
	ofs.close();
	return false;
} // tngud's part (store the structure in a file)

// load B+Tree from Students_score.idx
bool BPlusTree::loadTree() {
	char c;
	int size, bln;
	ifstream ifs("students_score.idx", ios::binary);
	InternalNode* nd;
	vector<TerminalNode*> tns;
	vector<InternalNode*> ins;
	do{
		c = ' ';
		bln = 0;
		size = 0;
		ifs.read((char*)&c, 1);
		ifs.read((char*)&bln, 4);
		ifs.read((char*)&size, 4);
		if(c=='T') {
			TerminalNode* nd = new TerminalNode();
			nd->allocatedBlockNumber = bln;
			nd->storedRecordNumber = size;

			for(int i = 0; i<size;++i){
				float score;
				ifs.read((char*)&score, 4);
				ifs.read((char*)&bln, 4);

				if (i != size) {
					nd->scores[i] = score;
					nd->studID[i] = bln;
				}
			}
			ifs.read((char*)&bln, 4);
			nd->nextTerminalNode = (TerminalNode*)bln;
			tns.push_back(nd);
		}
		else {
			nd = new InternalNode();
			nd->allocatedBlockNumber = bln;
			nd->storedRecordNumber = size;
			for(int i = 0; i<size; ++i) {
				float deli;
				ifs.read((char*)&bln, 4);
				ifs.read((char*)&deli, 4);
				if(i!=size){
					nd->scoreDeli[i] = deli;
					nd->branchs[i] = (Node*)bln;
				}
			}
			ins.push_back(nd);
		}
	} while (!ifs.eof());
	ins.pop_back();
	this->rootNode = ins.back();
	ins.pop_back();
	reCreateTree(rootNode,ins,tns);
	return true;
}  // tngud's part (read the structure from a file)

void BPlusTree::reCreateTree(Node* nd,vector<InternalNode*>& ins, vector<TerminalNode*> tns)
{
	if(nd->ifTerminal()) {
		TerminalNode* node = (TerminalNode*)nd;
		int num = (int)node->nextTerminalNode;
		if (num == 0)
			node->nextTerminalNode = NULL;
		else{
			int j = 0;
			for (; j < tns.size() && !(tns[j]->allocatedBlockNumber == num); ++j)
				;
			node->nextTerminalNode = tns[j];
		}
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
		}
		else {
			for(j=0; j<tns.size()&&!(tns[j]->allocatedBlockNumber==num); ++j)
				;
			if(j<tns.size()) {
				node->branchs[0] = tns[j];
			}
			terminal = true;
		}
		for(int i = 1; i<node->storedRecordNumber;++i){
			int num = (int)node->branchs[i];
			if(terminal){
				for (j = 0; j<tns.size() && !(tns[j]->allocatedBlockNumber == num); ++j)
					;
				node->branchs[i] = tns[j];
			}
			else {
				for (j = 0; j<ins.size() && !(ins[j]->allocatedBlockNumber != num); ++j)
					;
				node->branchs[i] = ins[j];
			}
		}
		for(int i = 0; i<node->storedRecordNumber; ++i) {
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
