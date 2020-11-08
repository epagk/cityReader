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
    	if (isspace(str.at(0)))
    		str.erase(str.begin());		// remove that space

    	cout << str << "\n\n";


    	vector<string> line = removeDupWord(str);

    	if (line.at(0).compare("<node") == 0)	// Infos about a node
    	{
    		cout << "We have a node here!" << endl;

    		size_t u_pos = str.find("user");
	    	size_t u_id = str.find("uid");
	    	string uname = str.substr (u_pos, (u_id - u_pos)-1 );
	    	uname = space2underscore(uname);
	    	str.replace(u_pos, (u_id - u_pos)-1, uname);
	    	cout << "New Line \n" << str << endl;

	    	line = removeDupWord(str);

    		size_t pos = line.at(1).find("=");      	 // position of "=" in str
  			string id = line.at(1).substr (pos+1);       // get the id of node
  			id.erase(id.begin()); id.erase(id.end()-1);	 // remove " "
    		cout << "id of node: " << id << endl;
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
	return 0;
}