#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>

using namespace std;

class Node
{
	private:
		string id;
		double lat;
		double lon;
		vector<Node*> connections; 

	public:
		Node() { };

		Node(string ident, double latitude, double longitude)
		{
			id = ident;
			lat = latitude;
			lon = longitude;
		}

		public:

			// Setter
			void setID(string ID) {
      			id = ID;
    		}

    		// Getter
    		string getID(){
    			return id;
    		}

    		// Setter
			void setLat(double lt) {
      			lat = lt;
    		}

    		// Getter
    		double getLat(){
    			return lat;
    		}

    		// Setter
			void setLon(double ln) {
      			lon = ln;
    		}

    		// Getter
    		double getLon(){
    			return lon;
    		}

    		void addConnection(Node* a)
		    {
		        connections.push_back(a);
		    }

		    vector<Node*> getConnections()
		    {
		        return connections;
		    }	
			
			void displayNode();
};

#endif