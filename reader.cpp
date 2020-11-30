#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include "node.h"
#include "cityGraph.h"

using namespace std;

vector<Node*> nodes;

void Node::displayNode()
{
    cout << "Node: " << id << endl;
    printf("\tLatitude: %.7lf \n",lat);
    printf("\tLongitude: %.7lf \n",lon);
    printf("\tConnections:\n");
    if (connections.size() == 0)
    	cout << "No Connections!" << endl;
    else
    {
    	for (int i = 0; i < connections.size(); ++i)
	    {
	    	Node* n = connections.at(i);
	    	cout << n->getID() << " ";
	    }
	    cout << endl;
    }
}

int main()
{
	parse_data("cities/rethymno.osm");
  	// point_edges();

  	for (int i = 0; i < nodes.size(); ++i)
  	{
    	Node* n = nodes.at(i);
    	cout << i << ") " << endl;
    	(*n).displayNode();
  	}

	return 0;
}