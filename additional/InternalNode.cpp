#include "InternalNode.h"

InternalNode::InternalNode ():Node()
{
}

InternalNode::InternalNode (Node * left, float separator, Node * right)
{
	scoreL.push_back (separator);
	branchL.push_back (left);
	branchL.push_back (right);
}

void InternalNode::print ()
{
	(*branchL.begin())->print ();
}

Node* InternalNode::insert (float score, char * id)
{
	list<float>::iterator it;
	list<Node*>::iterator bIt = branchL.begin ();
	for (it = scoreL.begin (); it != scoreL.end (); ++it, ++bIt) {
		if (*it >= score) break;
	}
	Node* temp = (*bIt)->insert (score, id);

	if (temp != (*bIt)) {		
		overflowSolve (bIt, it, temp);

		if (scoreL.size () > capacity) 
			return modifyNode ();
		
		else
			return this->insert (score, id);
	}
	return this;
}

void InternalNode::erase (float score, char * id)
{
	
}

vector<string> InternalNode::search (float lowRange, float HighRange)
{
	int i=0;
	list<Node*>::iterator branchIT = branchL.begin ();
	for (auto it = scoreL.begin (); it != scoreL.end (); ++it, ++branchIT) {
		if (*it > lowRange)
			break;		
	}
	return (*branchIT)->search (lowRange, HighRange);
}

float InternalNode::getFirstItem ()
{
	return *scoreL.begin();
}

void InternalNode::overflowSolve (list<Node*>::iterator branchLoc, list<float>::iterator scoreLocation, Node* temp)
{
	if (scoreLocation != scoreL.end ()) {
		scoreL.insert (scoreLocation, temp->getFirstItem ());
	}
	else {
		scoreL.push_back (temp->getFirstItem ());
	}

	if (*branchLoc != branchL.back ()) {
		++branchLoc;
		branchL.insert (branchLoc, temp);
	}
	else {
		branchL.push_back (temp);
	}
}

Node * InternalNode::modifyNode ()
{
	InternalNode* temp = new InternalNode ();
	auto sIt = scoreL.begin ();
	auto bIt = branchL.begin ();
	for (int i = 0; i < capacity / 2; ++i) {
		++sIt;
		++bIt;
	}
	++bIt;
	++sIt;
	float separator = *sIt;
	++bIt;
	++sIt;
	while(sIt != scoreL.end()){
		temp->scoreL.push_back (*sIt);
		++sIt;
	}
	while (bIt != branchL.end ()){
		temp->branchL.push_back (*bIt);
		++bIt;
	}

	while (scoreL.size () != capacity / 2)
		scoreL.pop_back ();
	while (branchL.size () != capacity / 2 +1)
		branchL.pop_back ();

	return temp;
}

