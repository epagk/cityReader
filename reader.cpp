#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// Replace spaces in a string with underscores
string space2underscore(string text) {
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

void parse_data(string filename)
{
	ifstream file(filename);
  	string str;

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

    	cout << str << "\n\n";
    	vector<string> line = removeDupWord(str);

    	if (line.at(0).compare("<node") == 0)	// Infos about a node
    	{
    		cout << "We have a node here!" << endl;

    		// We need to remove spaces from user's name
    		// in order to separate the infos easier
    		size_t u_pos = str.find("user");
	    	size_t u_id = str.find("uid");
	    	string uname = str.substr (u_pos, (u_id - u_pos)-1 );
	    	uname = space2underscore(uname);
	    	str.replace(u_pos, (u_id - u_pos)-1, uname);

	    	line = removeDupWord(str);

	    	// Extract node's id
    		size_t pos = line.at(1).find("=");      	 // position of "=" in str
  			string id = line.at(1).substr (pos+1);       // get the id of node
  			id.erase(id.begin()); id.erase(id.end()-1);	 // remove " "
    		cout << "id of node: " << id << endl;

    		// Exrtact node's longitude
    		pos = line.at(8).find("=");      	 	 		 // position of "=" in str
  			string lat = line.at(8).substr (pos+1);      	 // get the lat of node
  			lat.erase(lat.begin()); lat.erase(lat.end()-1);	 // remove " "
    		cout << "lat of node: " << lat << endl;

    		// Exrtact node's longitude
    		pos = line.at(9).find("=");      	 	 		 // position of "=" in str
  			string lon = line.at(9).substr (pos+1);      	 // get the lon of node
  			lon.erase(lon.begin()); lon.erase(lon.end()-1);	 // remove " "
  			cout << "lon of node: " << lon << endl;
    	}

    	for (int i = 0; i < line.size(); ++i)
	  	{
	  		str = line.at(i);
	  		cout << str << endl;
	  	}
	  	cout << endl;
  	}
}

int main()
{
	
	parse_data("test.osm");

	// string str = "35.5137157";
	// double lon = stod (str);

	return 0;
}