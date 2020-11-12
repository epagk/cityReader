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
    {
    	cout << "No Connections!" << endl;
    }
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

// Find all the edges on grpah of city for plot purpose
void point_edges()
{
  vector<pair<Node*, Node*>> edges;
  pair <Node*, Node*> edge;

  for (int i = 0; i < nodes.size(); ++i)
  {
    Node* n = nodes.at(i);
    for (int i = 0; i < (*n).getConnections()->size(); ++i)
    {
      Node* c = (*n).getConnections()->at(i); 
      edge = make_pair(n, c);
      edges.push_back(edge);
    }
  }

  for (int i = edges.size()-1; i > 0; --i)
  {
    edge = edges.at(i);
    string a, b, c, d;
    a = (edge.first)->getID();
    b = (edge.second)->getID();

    for (int j = i-1; j > -1; --j)
    {
      edge = edges.at(j);
      c = (edge.first)->getID();
      d = (edge.second)->getID();

      if ( a.compare(d) == 0 && b.compare(c) == 0 )
        edges.erase(edges.begin() + i);
    }
  }

  // Write infos of points and edges to a file
  // so we can plot the map of the city
  for (int i = 0; i < edges.size(); ++i)
  {
    edge = edges.at(i);

    stringstream a_stream;
    stringstream b_stream;
    stringstream c_stream;
    stringstream d_stream;

    a_stream << fixed << setprecision(7) << (edge.first)->getLon();
    b_stream << fixed << setprecision(7) << (edge.first)->getLat();
    c_stream << fixed << setprecision(7) << (edge.second)->getLon();
    d_stream << fixed << setprecision(7) << (edge.second)->getLat();

    string str = a_stream.str() + " " + b_stream.str() + " " + c_stream.str() + " " + d_stream.str();

    cout << str << endl;
  }
  
}

int main()
{
	parse_data("chania.osm");
  point_edges();

	return 0;
}