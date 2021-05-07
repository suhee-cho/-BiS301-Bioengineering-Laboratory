#include "DPmat.h"
#include "string.h"
#include "algorithm"


///////////////////////////// A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  Z  X
int scoring_mat[23][23] = { { 4,-1,-2,-2, 0,-1,-1, 0,-2,-1,-1,-1,-1,-2,-1, 1, 0,-3,-2, 0,-2,-1, 0},
                        	{-1, 5, 0,-2,-3, 1, 0,-2, 0,-3,-2, 2,-1,-3,-2,-1,-1,-3,-2,-3,-1, 0,-1},
                            {-2, 0, 6, 1,-3, 0, 0, 0, 1,-3,-3, 0,-2,-3,-2, 1, 0,-4,-2,-3, 3, 0,-1},
                            {-2,-2, 1, 6,-3, 0, 2,-1,-1,-3,-4,-1,-3,-3,-1, 0,-1,-4,-3,-3, 4, 1,-1},
                            { 0,-3,-3,-3, 9,-3,-4,-3,-3,-1,-1,-3,-1,-2,-3,-1,-1,-2,-2,-1,-3,-3,-2},
                            {-1, 1, 0, 0,-3, 5, 2,-2, 0,-3,-2, 1, 0,-3,-1, 0,-1,-2,-1,-2, 0, 3,-1},
                            {-1, 0, 0, 2,-4, 2, 5,-2, 0,-3,-3, 1,-2,-3,-1, 0,-1,-3,-2,-2, 1, 4,-1},
                            { 0,-2, 0,-1,-3,-2,-2, 6,-2,-4,-4,-2,-3,-3,-2, 0,-2,-2,-3,-3,-1,-2,-1},
                            {-2, 0, 1,-1,-3, 0, 0,-2, 8,-3,-3,-1,-2,-1,-2,-1,-2,-2, 2,-3, 0, 0,-1},
                            {-1,-3,-3,-3,-1,-3,-3,-4,-3, 4, 2,-3, 1, 0,-3,-2,-1,-3,-1, 3,-3,-3,-1},
                            {-1,-2,-3,-4,-1,-2,-3,-4,-3, 2, 4,-2, 2, 0,-3,-2,-1,-2,-1, 1,-4,-3,-1},
                            {-1, 2, 0,-1,-3, 1, 1,-2,-1,-3,-2, 5,-1,-3,-1, 0,-1,-3,-2,-2, 0, 1,-1},
                            {-1,-1,-2,-3,-1, 0,-2,-3,-2, 1, 2,-1, 5, 0,-2,-1,-1,-1,-1, 1,-3,-1,-1},
                            {-2,-3,-3,-3,-2,-3,-3,-3,-1, 0, 0,-3, 0, 6,-4,-2,-2, 1, 3,-1,-3,-3,-1},
                            {-1,-2,-2,-1,-3,-1,-1,-2,-2,-3,-3,-1,-2,-4, 7,-1,-1,-4,-3,-2,-2,-1,-2},
                            { 1,-1, 1, 0,-1, 0, 0, 0,-1,-2,-2, 0,-1,-2,-1, 4, 1,-3,-2,-2, 0, 0, 0},
                            { 0,-1, 0,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-2,-1, 1, 5,-2,-2, 0,-1,-1, 0},
                            {-3,-3,-4,-4,-2,-2,-3,-2,-2,-3,-2,-3,-1, 1,-4,-3,-2,11, 2,-3,-4,-3,-2},
                            {-2,-2,-2,-3,-2,-1,-2,-3, 2,-1,-1,-2,-1, 3,-3,-2,-2, 2, 7,-1,-3,-2,-1},
                            { 0,-3,-3,-3,-1,-2,-2,-3,-3, 3, 1,-2, 1,-1,-2,-2, 0,-3,-1, 4,-3,-2,-1},
							{-2,-1, 3, 4,-3, 0, 1,-1, 0,-3,-4, 0,-3,-3,-2, 0,-1,-4,-3,-3, 4, 1,-1},
							{-1, 0, 0, 1,-3, 3, 4,-2, 0,-3,-3, 1,-1,-3,-1, 0,-1,-3,-2,-2, 1, 4,-1},
							{ 0,-1,-1,-1,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2, 0, 0,-2,-1,-1,-1,-1,-1}};
							
DPmat::DPmat(){}
DPmat::DPmat(string input_x, string input_y)
{
	int i;
	LEN_OF_X = input_x.length();
	LEN_OF_Y = input_y.length();
	MAX_X = 0;
	MAX_Y = 0;
	MIN_Y = 0;
	MAX_score = 0;

	x = new int[LEN_OF_X];
	for(i = 0; i < LEN_OF_X; i++){
		x[i] = chr2int(input_x.at(i));
	}

	y = new int[LEN_OF_Y];
	//z = new int[LEN_OF_Y];
	for(i = 0; i < LEN_OF_Y; i++){
		y[i] = chr2int(input_y.at(i));
	}

	mat = new Cell* [LEN_OF_X+1];
	for (i = 0; i < LEN_OF_X+1; i++){
		mat[i] = new Cell[LEN_OF_Y+1];
	}

}

int DPmat::chr2int(char chr)
{
	if      ((chr == 'A')||(chr == 'a')) return 1;
	else if ((chr == 'R')||(chr == 'r')) return 2;
	else if ((chr == 'N')||(chr == 'n')) return 3;
	else if ((chr == 'D')||(chr == 'd')) return 4;
	else if ((chr == 'C')||(chr == 'c')) return 5;
	else if ((chr == 'Q')||(chr == 'q')) return 6;
	else if ((chr == 'E')||(chr == 'e')) return 7;
	else if ((chr == 'G')||(chr == 'g')) return 8;
	else if ((chr == 'H')||(chr == 'h')) return 9;
	else if ((chr == 'I')||(chr == 'i')) return 10;
	else if ((chr == 'L')||(chr == 'l')) return 11;
	else if ((chr == 'K')||(chr == 'k')) return 12;
	else if ((chr == 'M')||(chr == 'm')) return 13;
	else if ((chr == 'F')||(chr == 'f')) return 14;
	else if ((chr == 'P')||(chr == 'p')) return 15;
	else if ((chr == 'S')||(chr == 's')) return 16;
	else if ((chr == 'T')||(chr == 't')) return 17;
	else if ((chr == 'W')||(chr == 'w')) return 18;
	else if ((chr == 'Y')||(chr == 'y')) return 19;
	else if ((chr == 'V')||(chr == 'v')) return 20;
	else if ((chr == 'B')||(chr == 'b')) return 21;
	else if ((chr == 'Z')||(chr == 'z')) return 22;
	else if ((chr == 'X')||(chr == 'x')) return 23;
	else
	{
		cout << "Input Error! NOT valid aa code" << endl;
		cout << "Your chr in question: " << chr << endl;
		exit(0);
	}
}
char DPmat::int2chr(int num)
{
	if      (num == 0) return '-';
	else if (num == 1) return 'A';
	else if (num == 2) return 'R';
	else if (num == 3) return 'N';
	else if (num == 4) return 'D';
	else if (num == 5) return 'C';
	else if (num == 6) return 'Q';
	else if (num == 7) return 'E';
	else if (num == 8) return 'G';
	else if (num == 9) return 'H';
	else if (num == 10) return 'I';
	else if (num == 11) return 'L';
	else if (num == 12) return 'K';
	else if (num == 13) return 'M';
	else if (num == 14) return 'F';
	else if (num == 15) return 'P';
	else if (num == 16) return 'S';
	else if (num == 17) return 'T';
	else if (num == 18) return 'W';
	else if (num == 19) return 'Y';
	else if (num == 20) return 'V';
	else if (num == 21) return 'B';
	else if (num == 22) return 'Z';
	else if (num == 23) return 'X';
	else
	{
		cout << "Output Error! NOT valid number(0-20)" << endl;
		exit(0);
	}
}

void DPmat::print_z_chr()
{
	int i;
	string str = z.str();
	reverse(str.begin(), str.end());
	cout << "Aligned target sequence: " << endl;
	cout << str << endl;
	cout << "maximum optimal local alignment score"
		 << ": " << MAX_score << endl;
	cout << "This alignment corresponds to " << MIN_Y << "th or " << MIN_Y + 1
		 << "th to " << MAX_Y << "th residues of reference sequence.\n" << endl;
}

void DPmat::print_mat()
{
	cout << "<< score & flag of matrix>>"<< endl;
	int score;
	bool* flg;
	for(int i=0;i<LEN_OF_X+1;i++){
		for(int j=0;j<LEN_OF_Y+1;j++){
			score = mat[i][j].obtain_score();
			flg = mat[i][j].obtain_flag();
			cout << "[";
			for (int k=0; k<3; k++)
			{
				cout << flg[k] <<" ";
			}
			cout << score <<"]"<<"\t";
		}
		cout << "\n";
	}
	cout << endl;

}

void DPmat::fill_in_DPmat()
{
	bool flag[3] = {false, false, false}; // define default flag
	int score[3] = {0, 0, 0}; // define default score
	int final_score; // save final score value
	int i, j, k;
	// Initialize the first row and the first column of DPmat
	mat[0][0].set_scoreflag(0, flag);
	for (i = 1; i < LEN_OF_X+1; i++){
		flag[0] = false; // for first column, only third flag is true
		flag[2] = true;
		mat[i][0].set_scoreflag(0, flag);
	}
	for (j = 1; j < LEN_OF_Y+1; j++){
		flag[0] = true; // for first row, only first flag is true
		flag[2] = false;
		mat[0][j].set_scoreflag(0, flag);
	}
	for (i = 1; i < LEN_OF_X+1; i++){
		for (j = 1; j < LEN_OF_Y+1; j++){
			score[0] = mat[i][j-1].obtain_score() - 4; // when move right
			score[1] = mat[i-1][j-1].obtain_score() + scoring_mat[x[i-1]-1][y[j-1]-1]; // when move diagonal
			score[2] = mat[i-1][j].obtain_score() - 4; // when move down
			// final score is the maximum value of score array
			final_score = max(max(score[0], score[1]), score[2]);
			// the flag should point the direction where max score can be obtained
			for(k = 0; k < 3;k++) flag[k] = (score[k] == final_score);
			if (final_score < 0) final_score = 0;
			if (final_score >= MAX_score) {
				MAX_score = final_score;
				MAX_X = i;
				MAX_Y = j;
			}
			// determine Cell with final score and new flag
			mat[i][j].set_scoreflag(final_score, flag);
		}
	}
}

void DPmat::trace_back(int trace_back_i, int trace_back_j)
{	
	Cell target = mat[trace_back_i][trace_back_j];;
	bool* flag = new bool[3];
	flag = target.obtain_flag();

	if (target.obtain_score() == 0){
		MIN_Y = trace_back_j;
		DPmat::print_z_chr();
		return;
	}
	
	// trace back the direction from mat[LEN_OF_X][LEN_OF_Y] by recursive process
	// modify z while tracing
	if (flag[1]) { // diagonal case
		z << int2chr(x[trace_back_i-1]);
		trace_back(trace_back_i-1, trace_back_j-1);
	}
	else if (flag[2]) { // upward. y is blank
		z << int2chr(x[trace_back_i-1]);
		trace_back(trace_back_i-1, trace_back_j);
	}
	else if (flag[0]) { //left. x is blank
		z << "-";
		trace_back(trace_back_i, trace_back_j-1);
	}
	
}

DPmat::~DPmat()
{
	delete [] x;
	delete [] y;

	for(int i = 0; i<LEN_OF_X+1; i++) {
		delete [] mat[i];
	}
	delete [] mat;

}



