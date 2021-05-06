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
	int len[6];
	char* seq[6];
	int i;

	texts[0].open("Query.txt");
	for (i = 1; i < 6; i++){
		stringstream filename;
	    filename << "templates_0" << i << ".txt";
    	texts[i].open(filename.str());
	}
	for (i = 0; i < 6; i++){
		texts[i].ignore(100, '\n'); // remove header: first line
		//len[i] = texts[i].tellg();
		string temp((istreambuf_iterator<char>(texts[i])),
							istreambuf_iterator<char>());
		temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
		strings[i] = temp;
		len[i] = strings[i].size() + 1;
		seq[i] = new char[len[i]];
		copy(strings[i].begin(), strings[i].end(), seq[i]); // string to char array
		seq[i][strings[i].size()] = '\0';
	}

	for (i = 1; i <= 6; i++){
		DPmat SA(seq[i],seq[0]);
		SA.fill_in_DPmat();
		cout << "maximum optimal local alignment score for templates_0"
			 << i << ": " << SA.get_MAX_score() << endl;
	}

	//cout << "MAX_X: " << SA.get_MAX_X() << endl;
	//cout << "MAX_Y: " << SA.get_MAX_Y() << endl;
	// SA.trace_back(SA.get_MAX_X(), SA.get_MAX_Y());
	// delete[] seq;

	return 0;
}



