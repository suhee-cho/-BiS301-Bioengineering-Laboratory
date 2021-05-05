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
	//cout << seq[0][len[0]] << endl;
/*
	for (int i = 0; i < 6; i ++){
		cout << seq[i] << endl;
	}
*/
	//char seq_x[len[1]] = seq[1];
	//char seq_y[len[0]] = seq[0];

	DPmat SA(seq[1],seq[0]);
	SA.fill_in_DPmat();

	//SA.print_mat();
	//cout<<"y: "<<seq[0]<<endl;
    //cout<<"x: "<<seq[1]<<endl<<endl;
	cout << SA.get_MAX_X() << endl;
	cout << SA.get_MAX_Y() << endl;
	cout << SA.get_LEN_Y() << endl;

	SA.trace_back(SA.get_MAX_X(), SA.get_MAX_Y(), SA.get_LEN_Y()-1);
	delete[] seq;

	return 0;
}



