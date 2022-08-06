#include <iostream>
#include <stdlib.h>

using namespace std;

#ifndef __TEXT_H__
#define __TEXT_H__

class text
{
public:
	text();
	int obtain_length();
	char* obtain_flag();

private:
	string str;
	int length;
    char char_arr[length];
};

#endif