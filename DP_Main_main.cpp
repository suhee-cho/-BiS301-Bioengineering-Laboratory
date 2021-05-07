#include "Cell.h"
#include "DPmat.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
#include <vector>
using namespace std;

int main()
{
	ifstream texts[201];
	string strings[201];
	string line;
	vector <pair <int, string>> score_idx;
	int i;

	// import query sequence, and save it in strings[0]
	texts[0].open("mainlab_file\\Query.fa");
	getline(texts[0], line); // remove header
	getline(texts[0], line);
	strings[0].append(line);

	// import template sequences
	for (i = 1; i < 201; i++){
		// first, open files
		string filename = "template_";
		char num[5];
		sprintf(num, "%03d", i);
	    filename += num;
    	texts[i].open("mainlab_file\\" + filename + ".fa");

		// remove useless lines including header.
		while (getline(texts[i], line)){
			if (line.empty()|line.front() == '>') continue;
			strings[i].append(line);
		}

		// Then calculate each template's alignment score
		// and put it into score_idx vector
		DPmat SA(strings[i], strings[0]);
		SA.fill_in_DPmat();
		score_idx.push_back(make_pair(SA.get_MAX_score(), filename));	
	}

	// sort vector elements with score
	sort(score_idx.begin(), score_idx.end());

	// print the result in desired form, in descending order
	for (i = 0; i < 10; i++){
		cout << i << " " << score_idx[199-i].second << " score: " 
		<< score_idx[199-i].first << endl;
	}

	return 0;
}
