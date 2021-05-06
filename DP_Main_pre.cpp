#include "Cell.h"
#include "DPmat.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

int main()
{
	ifstream texts[6];
	string strings[6];
	char* seq[6];
	int i;

	for (i = 0; i < 6; i++){
		if (i == 0){
			texts[i].open("Query.txt");
		}
		else {
			stringstream filename;
	    	filename << "templates_0" << i << ".txt";
    		texts[i].open(filename.str());
		}
		texts[i].ignore(100, '\n'); // remove header: first line
		string temp((istreambuf_iterator<char>(texts[i])),
							istreambuf_iterator<char>());
		temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
		strings[i] = temp;
		seq[i] = new char[strings[i].size() + 1];
		copy(strings[i].begin(), strings[i].end(), seq[i]); // string to char array
		seq[i][strings[i].size()] = '\0';
		if (i > 0){
			DPmat SA(seq[i],seq[0]);
			SA.fill_in_DPmat();
			cout << "##########Templates_0" << i << "##########" << endl;
	
			SA.trace_back(SA.get_MAX_X(), SA.get_MAX_Y());
		}
	}
	return 0;
}



