#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "node.h"
#include "cityGraph.h"

using namespace std;

vector<Node> nodes;

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

// Replace spaces in a string with underscores
string space2underscore(string text) 
{
    for(string::iterator it = text.begin(); it != text.end(); ++it) {
        if(*it == ' ') {
            *it = '_';
        }
    }
    return text;
}

// Separate every line by spaces to get infos
// for id, longitude and latitude
vector<string> removeDupWord(string str)
{
	vector<string> v;
    string word = "";

    for (auto x : str) 
    {
        if (x == ' ')
        {
        	v.push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    v.push_back(word);
    return v;
}

// create pairs of nodes which are connected
void find_connections(string filename)
{
	ifstream file(filename);
	string str;
	string prevID = "0";
	Node* prevNode = getNode(nodes, prevID);

	while (getline(file, str))
	{
		vector<string> line = removeDupWord(str);

		if (line.at(0).compare("<tag") == 0)
			continue;

		if (line.at(0).compare("<way") == 0){
			prevID = "0";
			prevNode = getNode(nodes, prevID);
		}

		if (line.at(0).compare("<nd") == 0)
		{
			// Get node's id
			size_t a = line.at(1).find("=");
		    size_t b = line.at(1).find("/");
		    string id = line.at(1).substr (a+1, (b - a)-1 );
		    id.erase(id.begin()); id.erase(id.end()-1);

		    if (prevID.compare("0") != 0)
	    	{
	    		Node* a = getNode(nodes,id);
	    		a->addConnection(prevNode);
	    		prevNode->addConnection(a);	
	    	}

	    	prevID = id;
	    	prevNode = getNode(nodes, prevID);
		}
	}

	remove_duplicates(nodes);
}

// read the .osm file extracted from OpenStreetMap
// and record all the nodes with their longitude and latitude
void parse_data(string filename)
{
	ifstream file(filename);
  	string str;
  	string connections_file = "roads.txt";

	ofstream MyFile(connections_file);

  	while (getline(file, str)) 
  	{
  		// If a line starts with spaces
  		// we don't want that spaces
		for (int i = 0; i < str.size(); ++i)
		{
			if (isspace(str.at(i))){
				str.erase(str.begin());		// remove front spaces
				i--;
			}
			else
				break;
		}	

    	// cout << str << "\n\n";
    	vector<string> line = removeDupWord(str);

    	if (line.at(0).compare("<node") == 0)	// Infos about a node
    	{
    		Node nd;

    		// We need to remove spaces from user's name
    		// in order to separate the infos easier
    		size_t u_pos = str.find("user=");
	    	size_t u_id = str.find("uid=");
	    	string uname = str.substr (u_pos, (u_id - u_pos)-1 );
	    	uname = space2underscore(uname);
	    	str.replace(u_pos, (u_id - u_pos)-1, uname);

	    	line = removeDupWord(str);	// Separate line by spaces

	    	// Extract node's id
    		size_t pos = line.at(1).find("=");      	 // position of "=" in str
  			string id = line.at(1).substr (pos+1);       // get the id of node
  			id.erase(id.begin()); id.erase(id.end()-1);	 // remove " "

    		nd.setID(id);	// Set node's id

    		// Exrtact node's latitude
    		pos = line.at(8).find("=");      	 	 		 // position of "=" in str
  			string lat = line.at(8).substr (pos+1);      	 // get the lat of node
  			lat.erase(lat.begin()); lat.erase(lat.end()-1);	 // remove " "
    		double lat_d = stod (lat);

    		nd.setLat(lat_d);	// Set node's latitude as bouble
  			
    		// Exrtact node's longitude
    		pos = line.at(9).find("=");      	 	 		 // position of "=" in str
  			string ln = line.at(9).substr (pos+1);      	 // get the lon of node
  			string lon;
  			for (int i = 0; i < ln.size(); ++i)
			{
				if (isdigit(ln.at(i)) || ln.at(i) == '.')
					lon.push_back(ln.at(i));
			}
  			double lon_d = stod (lon);

    		nd.setLon(lon_d);	// Set node's longtitude as bouble

    		// nd.displayNode();
    		nodes.push_back(nd);
     	}
     	else
     	{
     		if (line.at(0).compare("<way") == 0 || line.at(0).compare("<nd") == 0 || line.at(0).compare("<tag") == 0)
     		{
     			MyFile << str << "\n";
     		}
     	}

    // 	for (int i = 0; i < line.size(); ++i)
	  	// {
	  	// 	str = line.at(i);
	  	// 	cout << str << endl;
	  	// }
	  	// cout << endl;
  	}

  	MyFile.close();
  	find_connections(connections_file);
}


int main()
{
	
	parse_data("chania.osm");

	for (int i = 0; i < nodes.size(); ++i)
	{
		cout << i << ")" << endl;
		Node n = nodes.at(i);
		n.displayNode();
	}

	// Node* n = getNode(nodes, "29453727");

	// (*n).displayNode();

	cout << "\n\nNum of nodes: " << nodes.size() << endl;

	return 0;
}