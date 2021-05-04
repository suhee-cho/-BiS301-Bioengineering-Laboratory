#include "DPmat.h"
#include "string.h"
#include "algorithm"

////////////////////////// A   T   G   C
int scoring_mat[4][4] = {{ 5, -1, -2, -1},
						 {-1,  5, -2, -2},
						 {-2, -2,  5, -3},
						 {-1, -2, -3,  5}};

DPmat::DPmat(){}
DPmat::DPmat(char* input_x, char* input_y)
{
	int i;
	LEN_OF_X = strlen(input_x);
	LEN_OF_Y = strlen(input_y);

	x = new int[LEN_OF_X];
	for(i = 0; i < LEN_OF_X; i++){
		x[i] = chr2int(input_x[i]);
	}

	y = new int[LEN_OF_Y];
	z = new int[LEN_OF_Y];
	for(i = 0; i < LEN_OF_Y; i++){
		y[i] = chr2int(input_y[i]);
		z[i] = 0;
	}

	mat = new Cell* [LEN_OF_X+1];
	for (i = 0; i < LEN_OF_X+1; i++){
		mat[i] = new Cell[LEN_OF_Y+1];
	}

}

int DPmat::chr2int(char chr)
{
	if      ((chr == 'A')||(chr == 'a')) return 1;
	else if ((chr == 'T')||(chr == 't')) return 2;
	else if ((chr == 'G')||(chr == 'g')) return 3;
	else if ((chr == 'C')||(chr == 'c')) return 4;
	else
	{
		cout << "Input Error! NOT valid character(A,T,G,C)" << endl;
		exit(0);
	}
}
char DPmat::int2chr(int num)
{
	if      (num == 0) return '-';
	else if (num == 1) return 'A';
	else if (num == 2) return 'T';
	else if (num == 3) return 'G';
	else if (num == 4) return 'C';
	else
	{
		cout << "Output Error! NOT valid number(0,1,2,3,4)" << endl;
		exit(0);
	}

}
void DPmat::print_z_chr()
{
	int i;
	cout << "z: ";
	for (i = 0; i < LEN_OF_Y; i++){
		cout << int2chr(z[i]);
	}
    // print z (A, T, G, C, -)
    // print score of mat[LEN_OF_X][LEN_OF_Y]
	cout << "	score: " << mat[LEN_OF_X][LEN_OF_Y].obtain_score() << endl;
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
		mat[i][0].set_scoreflag(-6*i, flag);
	}
	for (j = 1; j < LEN_OF_Y+1; j++){
		flag[0] = true; // for first row, only first flag is true
		flag[2] = false;
		mat[0][j].set_scoreflag(-6*j, flag);
	}
	for (i = 1; i < LEN_OF_X+1; i++){
		for (j = 1; j < LEN_OF_Y+1; j++){
			score[0] = mat[i][j-1].obtain_score() - 6; // when move right
			score[1] = mat[i-1][j-1].obtain_score() + scoring_mat[x[i-1]-1][y[j-1]-1]; // when move diagonal
			score[2] = mat[i-1][j].obtain_score() - 6; // when move down
			// final score is the maximum value of score array
			final_score = max(max(score[0], score[1]), score[2]);
			// the flag should point the direction where max score can be obtained
			for(k = 0; k < 3;k++) flag[k] = (score[k] == final_score);
			// determine Cell with final score and new flag
			mat[i][j].set_scoreflag(final_score, flag);
		}
	}
}

void DPmat::trace_back(int trace_back_i, int trace_back_j, int trace_back_cnt)
{	
	//trace_back_cnt = z index
	Cell target;
	bool* flag = new bool[3];
	flag = target.obtain_flag();

	if (trace_back_cnt < 0){
		DPmat::print_z_chr();
		return;
	}
	
	target = mat[trace_back_i][trace_back_j];
	// trace back the direction from mat[LEN_OF_X][LEN_OF_Y] by recursive process
	// modify z while tracing
	if (flag[1]) { // diagonal case
		z[trace_back_cnt] = x[trace_back_i-1];
		trace_back(trace_back_i-1, trace_back_j-1, trace_back_cnt-1);
	}
	if (flag[0]) { //left. x is blank
		z[trace_back_cnt] = 0;
		trace_back(trace_back_i, trace_back_j-1, trace_back_cnt-1);
	}
	if (flag[2]) { // upward. y is blank
		z[trace_back_cnt] = x[trace_back_i-1];
		trace_back(trace_back_i-1, trace_back_j, trace_back_cnt-1);
	}
}

DPmat::~DPmat()
{
	delete [] x;
	delete [] y;
	delete [] z;

	for(int i = 0; i<LEN_OF_X+1; i++) {
		delete [] mat[i];
	}
	delete [] mat;

}



