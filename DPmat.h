#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "Cell.h"

using namespace std;

#ifndef __DPMAT_H__
#define __DPMAT_H__

class DPmat
{
public:
	DPmat();
	DPmat(char* input_x, char* input_y);
	void print_z_chr();
	void fill_in_DPmat();
	void print_mat();
	void trace_back(int trace_back_i, int trace_back_j);
	int chr2int(char chr);
	char int2chr(int num);

	int get_LEN_X(){ return LEN_OF_X; }
	int get_LEN_Y(){ return LEN_OF_Y; }

	int get_MAX_X() { return MAX_X; }
	int get_MAX_Y() { return MAX_Y; }
	int get_MAX_score() { return MAX_score; }

	~DPmat();

private:
	int* x;
	int* y;
	//int* z;
	stringstream z;

	int LEN_OF_X;
	int LEN_OF_Y;

	int MAX_X;
	int MAX_Y;
	int MAX_score;

	Cell** mat;

};

#endif

