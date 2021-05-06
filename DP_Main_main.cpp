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
	char* seq[201];
	vector <pair <int, int>> score_idx;
	int i;
	//texts[0].open("mainlab_file\\template_001.fa");
	//getline(texts[0], line);
	//getline(texts[0], line);
    // Now begin your useful code
        // This will just over write the first line read
    
	//cout << strings[0] << endl;
	// texts[0].ignore(100, '\n'); // remove header: first line
	//string temp((istreambuf_iterator<char>(texts[0])),istreambuf_iterator<char>());
	//cout << strings[0] << endl;

	for (i = 0; i < 201; i++){
		if (i == 0){
			texts[0].open("mainlab_file\\Query.fa");
			getline(texts[0], strings[0]);
			getline(texts[0], strings[0]);
			getline(texts[0], strings[0]);
		}
		else{
			stringstream filename;
	    	filename << "mainlab_file\\template_00" << i << ".fa";
    		texts[i].open(filename.str());
			getline(texts[i], strings[i]);
			getline(texts[i], strings[i]);
			getline(texts[i], strings[i]);
		}

		seq[i] = new char[strings[i].size() + 1];
		copy(strings[i].begin(), strings[i].end(), seq[i]); // string to char array
		seq[i][strings[i].size()] = '\0';

		if (i > 0){
			DPmat SA(seq[i],seq[0]);
			SA.fill_in_DPmat();
			score_idx.push_back(make_pair(SA.get_MAX_score(), i));
		}
	}
	//sort(score_idx.begin(), score_idx.end());

	for (i = 0; i < 200; i++){
		cout << i+1 << " template_" << score_idx[i].second << " score: " 
		<< score_idx[i].first << endl;
	}

	return 0;
}



