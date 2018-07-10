#include "BPlusTree.h"
#include"InternalNode.h"
#include "TerminalNode.h"

BPlusTree::BPlusTree ()
{
	rootNode = new TerminalNode();
}

void BPlusTree::insert (float score, char * find)
{
	Node* retV = rootNode->insert (score, find);
	if (retV != rootNode) {
		float separator = retV->getFirstItem ();		
		Node* temp = new InternalNode (rootNode, separator,retV);
		rootNode = temp;		
		rootNode->insert (score, find);
		first = false;
	}
}

vector<string> BPlusTree::search (float lowRange, float highRange)
{
	return rootNode->search (lowRange, highRange);
}

void BPlusTree::erase (float score, char * find)
{
	rootNode->erase (score, find);
}

void BPlusTree::print ()
{
	rootNode->print ();
}


