#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include "TerminalNode.h"

using namespace std;

TerminalNode::TerminalNode ():Node()
{
	size = 0;
	nextNode = nullptr;
}

float TerminalNode::getFirstItem ()
{	
	return score[0];
}

Node* TerminalNode::insert (float score, char * id)
{
// insert½Ã overflow ¹ß»ý
	if (size == capacity) {
		return modifyNode ();
	}
	int i;
	for (i = 0; i < size; ++i) {
		if (this->score[i] >= score) {
			break;
		}
	}	
	if (i != size) {
		this->shiftContainer (i);
	}
	this->score[i] = score;
	strcpy (this->id[i], id);
	++size;
	return this;
}

void TerminalNode::erase (float score, char * id)
{
	for (int i = 0; i < size; ++i) {
		if (this->score[i] == score && strcmp (this->id[i], id) == 0) {
			for (int j = i; j < size; ++j) {
				this->score[j] = this->score[j+1];
				strcpy (this->id[j], this->id[j + 1]);
			}
			--size;
			break;
		}
	}
}

vector<string> TerminalNode::search (float lowRange, float HighRange)
{
	vector<string> temp;
	for (int i = 0; i < size; ++i) {
		if (score[i] < lowRange)
			continue;
		else if (lowRange <= score[i] && score[i]<=HighRange) {
			temp.push_back (id[i]);
		}
		else
			return temp;
	}
	if (nextNode == NULL)
		return temp;
	vector<string> nextTemp = nextNode->search(lowRange,HighRange);
	for (int i = 0; i < nextTemp.size (); ++i) {
		temp.push_back (nextTemp[i]);
	}

	return temp;
}

void TerminalNode::print ()
{
	for (int i = 0; i < size; ++i) {
		cout << id[i] << ", "<<score[i]<<" ";
	}
	cout << "**********************"<<endl;
	if (nextNode != nullptr)
		nextNode->print ();
}

void TerminalNode::shiftContainer (int idx)
{
	for (int i = size - 1; i >= idx; --i) {
		score[i + 1] = score[i];
		strcpy (id[i + 1], id[i]);		
	}
}

Node* TerminalNode::modifyNode ()
{
	TerminalNode* temp = new TerminalNode ();
	while (size != capacity / 2) {
		int middleIDX = capacity / 2;		
		temp->insert (score[middleIDX], id[middleIDX]);
		this->erase (score[middleIDX], id[middleIDX]);
	}
	temp->nextNode = this->nextNode;
	this->nextNode = temp;
	return temp;
}
