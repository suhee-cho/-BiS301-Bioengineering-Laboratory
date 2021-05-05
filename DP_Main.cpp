#include "Cell.h"
#include "DPmat.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
//#include <cctype>
using namespace std;

int main()
{
	/*
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
	*/
	char seq_x[] = "TTCCG";
	char seq_y[] = "TAACTCG";

	DPmat SA(seq_x,seq_y);
	SA.fill_in_DPmat();

	SA.print_mat();
	cout<<"y: "<<seq_y<<endl;
    cout<<"x: "<<seq_x<<endl<<endl;
	cout << SA.get_MAX_X() << endl;
	cout << SA.get_MAX_Y() << endl;
	cout << SA.get_MAX_score() << endl;

	SA.trace_back(SA.get_MAX_X(), SA.get_MAX_Y());
	// delete[] seq;

	return 0;
}



