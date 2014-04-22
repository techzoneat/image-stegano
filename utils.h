#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <sstream>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

using namespace std;


string int_to_string(int number)
{
   stringstream ss;
   ss << number;
   return ss.str();
}

uint single_char_to_int(char c)
{
	switch(c)
	{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'A':
		case 'a':
			return 0xa;
		case 'B':
		case 'b':
			return 0xb;
		case 'C':
		case 'c':
			return 0xc;
		case 'D':
		case 'd':
			return 0xd;
		case 'E':
		case 'e':
			return 0xe;
		case 'F':
		case 'f':
			return 0xf;
	}
	return 0;
}

#endif // UTILS_H
