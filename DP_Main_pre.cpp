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
	string line;
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
		while (getline(texts[i], line)){
			if (line.empty()|line.front() == '>') continue;
			strings[i].append(line);
		}
		if (i > 0){
			DPmat SA(strings[i],strings[0]);
			SA.fill_in_DPmat();
			cout << "##########Templates_0" << i << "##########" << endl;
			SA.trace_back(SA.get_MAX_X(), SA.get_MAX_Y());
		}
	}
	return 0;
}



