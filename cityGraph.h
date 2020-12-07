#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include "node.h"

using namespace std;

extern vector<Node*> nodes;

Node* getNode(vector<Node*> &v, string ID)
{	
	Node* it;

	for (unsigned n = 0; n < v.size(); n++)
	{	
		it = v.at(n);
		if (it->getID().compare(ID) == 0)
			return it;
	}

	Node nd("0", -1, -1);		// if given ID does not exist
	Node *nd_r = &nd;
	
	return nd_r;
}

// Replace spaces in a string with underscores
string space2underscore(string text) 
{
    for(string::iterator it = text.begin(); it != text.end(); ++it) {
        if(*it == ' ')
            *it = '_';
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

// If a connection appears more than once, we want to
// remove the extra appearances. Also we remove nodes which
// have no connections at all
void remove_duplicates(vector<Node*> &v)
{
	Node* it;

	for (unsigned n = 0; n < v.size(); n++)
	{
		vector<int> indexes;
		vector<string> listIDs;

		it = v.at(n);
		vector<Node*>* conn = it->getConnections();

		// Keep indexes of nodes without connections to delete
		// them later. Once they are not part of the road
		if (conn->empty())
			continue;

		for (int i = 0; i < conn->size()-1; ++i)
		{
			Node* c = conn->at(i);

			if ( count(listIDs.begin(), listIDs.end(), (*c).getID()) )	// Already checked that ID
				continue;	

			for (int j = i+1; j < conn->size(); ++j)
			{
				Node* d = conn->at(j);
				if ( (*c).getID().compare((*d).getID()) == 0 )
					indexes.push_back(j);
			}

			listIDs.push_back((*c).getID());
		}

		sort(indexes.begin(), indexes.end(), greater<int>()); // Sort indexes in descending order

		for (int i = 0; i < indexes.size(); ++i)
			conn->erase (conn->begin()+indexes.at(i));
	}

	for (int i = nodes.size()-1; i > -1; --i)
	{
		Node* n = nodes.at(i);
		if((*n).getConnections()->empty())
			nodes.erase(nodes.begin()+i);
	}
}

Node* getInitNode()
{
	srand ( time(NULL) );

	int max = -1;
	vector<Node*> candidates;

	for (int i = 0; i < nodes.size(); ++i)
	{
		Node* n = nodes.at(i);
		int h = (*n).getConnections()->size();

		if (h < max)
		  continue;

		if (h > max){
		  max = h;
		  candidates.clear();
		  candidates.push_back(n);
		}
		else
		  candidates.push_back(n);
	}

	int r = rand() % candidates.size();
	return candidates.at(r);
}

// Use BFS in order to get rid of disconnected nodes
void BFS() 
{ 
	vector<Node*> inGraph;

	// Mark all the vertices as not visited 
    list<Node*> visited; 
  
    // Create a queue for BFS 
    list<Node*> queue; 
  
  	// Pick randomly a node
    // Mark the current node as visited and enqueue it 
    Node* s = getInitNode();

    visited.push_back(s);
    queue.push_back(s); 
    inGraph.push_back(s);
  
    while(!queue.empty()) 
    { 
        // Dequeue a vertex from queue and print it 
        s = queue.front(); 
        queue.pop_front(); 
  
        // Get all adjacent vertices of the dequeued 
        // vertex s. If a adjacent has not been visited,  
        // then mark it visited and enqueue it 
        Node* n;
		for (unsigned i = 0; i < (*s).getConnections()->size(); ++i)
		{
			n = (*s).getConnections()->at(i);
			if (find(visited.begin(), visited.end(), n) != visited.end() == 0 )
			{
				visited.push_back(n);
				queue.push_back(n);
				inGraph.push_back(n);
			}
		} 
    } 

    nodes.clear();
    auto it = next(inGraph.begin(), inGraph.size());
	move(inGraph.begin(), it, back_inserter(nodes));
	inGraph.erase(inGraph.begin(), it);
}

// create pairs of nodes which are connected
void find_connections(string filename)
{
	ifstream file(filename);
	string str;
	string prevID = "0";
	Node* prevNode = getNode(nodes, prevID);
	vector<Node*> road;
	size_t a, b;

	while (getline(file, str))
	{
		vector<string> line = removeDupWord(str);

		if (line.at(0).compare("<way") == 0)	// Keep record of nodes for a road
		{
			road.clear();
			prevID = "0";
			prevNode = getNode(nodes, prevID);
		}

		if (line.at(0).compare("<nd") == 0)
		{
			// Get node's id
			a = line.at(1).find("=");
		    b = line.at(1).find("/");
		    string id = line.at(1).substr (a+1, (b - a)-1 );
		    id.erase(id.begin()); id.erase(id.end()-1);

		    Node* a = getNode(nodes,id);
		    road.push_back(a);
		}

		if (line.at(0).compare("<tag") == 0)	// Don't keep record for unreachable roads
		{
			if (road.empty())
				continue;

			a = line.at(2).find("=");
		    b = line.at(2).find("/");
		    string tag = line.at(2).substr (a+1, (b - a)-1 );

			if ( tag.compare("\"ferry\"") == 0  || tag.compare("\"footway\"") == 0 ||  tag.compare("\"pedestrian\"") == 0 || tag.compare("\"steps\"") == 0 
				|| tag.compare("\"hotel\"") == 0 || tag.compare("\"path\"") == 0 || tag.compare("\"island\"") == 0 || tag.compare("\"breakwater\"") == 0 )
			{
				road.clear();
			}

		    string spec = line.at(1);

			if ( spec.compare("k=\"leisure\"") == 0 || spec.compare("k=\"building\"") == 0 || spec.compare("k=\"natural\"") == 0 || spec.compare("k=\"landuse\"") == 0 
				|| spec.compare("k=\"parking\"") == 0 || spec.compare("k=\"amenity\"") == 0 || spec.compare("k=\"website\"") == 0 || spec.compare("k=\"man_made\"") == 0
				|| spec.compare("k=\"waterway\"") == 0 )
			{
				road.clear();
			}


		    
		}

		if (line.at(0).compare("</way>") == 0)	// Save an acceptable road
		{
			if ( road.empty() )
				continue;
			
			Node* n1 = road.at(0);
	    	for (int i = 1; i < road.size(); ++i)
	    	{
	    		Node* n2 = road.at(i);
	    		n1->addConnection(n2);
    			n2->addConnection(n1);
    			n1 = road.at(i);
	    	}
		}
	}
	remove_duplicates(nodes);
	BFS();	// Delete disconnected nodes from main graph
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

	  	vector<string> line = removeDupWord(str);

	  	if (line.at(0).compare("<node") == 0)	// Infos about a node
	  	{
	  		Node* nd = new Node();

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

	  		(*nd).setID(id);	// Set node's id

	  		// Exrtact node's latitude
	  		pos = line.at(8).find("=");      	 	 		 // position of "=" in str
			string lat = line.at(8).substr (pos+1);      	 // get the lat of node
			lat.erase(lat.begin()); lat.erase(lat.end()-1);	 // remove " "
	  		double lat_d = stod (lat);

	  		(*nd).setLat(lat_d);	// Set node's latitude as bouble
				
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

	  		(*nd).setLon(lon_d);	// Set node's longtitude as bouble

	  		nodes.push_back(nd);
	   	}
	   	else
	   	{
	   		// Write infos about ways and roads in another file in order to process it later
	   		if (line.at(0).compare("<way") == 0 || line.at(0).compare("<nd") == 0 || line.at(0).compare("<tag") == 0 || line.at(0).compare("</way>") == 0)
	   		   MyFile << str << "\n";
	   	}
	}

	MyFile.close();
	find_connections(connections_file);

	// Delete the File. We took all the usefull
    // infos and we don't need it anymore
    int n = connections_file.length();
    char file_deletion[n + 1];
    strcpy(file_deletion, connections_file.c_str());
    remove(file_deletion);
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