#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include "node.h"

using namespace std;

extern vector<Node> nodes;

Node* getNode(vector<Node> &v, string ID)
{	
	Node* it;

	for (unsigned n = 0; n < v.size(); n++)
	{	
		it = &(v.at(n));
		if (it->getID().compare(ID) == 0){
			return it;
		}
	}

	Node nd("0", -1, -1);		// if given ID does not exist
	Node *nd_r = &nd;
	
	return nd_r;
}

// Check of id belongs in already processed IDs 
bool inList(vector<string> &listIDs, string id)
{
	for (int i = 0; i < listIDs.size(); ++i)
	{
		string curID = listIDs.at(i);
		if (curID.compare(id) == 0 )
			return true;
	}

	return false;
}

// If a connection appears more than once, we want to
// remove the extra appearances
void remove_duplicates(vector<Node> &v)
{
	Node* it;

	for (unsigned n = 0; n < v.size(); n++)
	{
		vector<int> indexes;
		vector<string> listIDs;

		it = &(v.at(n));
		vector<Node*> conn = it->getConnections();

		for (int i = 0; i < conn.size()-1; ++i)
		{
			Node* c = conn.at(i);

			if ( count(listIDs.begin(), listIDs.end(), (*c).getID()) )	// Already checked that ID
				continue;	

			for (int j = i+1; j < conn.size(); ++j)
			{
				Node* d = conn.at(j);
				if ( (*c).getID().compare((*d).getID()) == 0 )
					indexes.push_back(j);
			}

			listIDs.push_back((*c).getID());
		}

		sort(indexes.begin(), indexes.end(), greater<int>()); // Sort indexes in descending order

		for (int i = 0; i < indexes.size(); ++i)
			conn.erase (conn.begin()+indexes.at(i));
	}
}