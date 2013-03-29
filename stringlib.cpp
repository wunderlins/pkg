//============================================================================
// Name        : stringlib.cpp
// Author      : Simon Wunderlin
// Version     : $Id: stringlib.cpp 124 2011-04-25 18:03:21Z wus $
// Copyright   : GPL2
// Description : string manipulation collection
//============================================================================

#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>

/** 
 * convert string to char array
 */
char * string2char(std::string str) {
	char * cstr; 
	cstr = new char[str.size()+1];
	strcpy(cstr, str.c_str());
	return cstr;
}

/**
 * convert int to char array
 */
void int2char(int i, char * c) {
	sprintf(c, "%d", i);
}

/**
 * left trim
 *
 * remove all spaces at the beginning of a string
 */
std::string ltrim(std::string str) {
	int i=0;
	do {i++;} while(str.at(i) == ' ');
	return str.substr(i);
}

/** 
 * Find the 1st occurance of '=' in string
 */
int first_index_of(const char search, std::string line) {
	return int(line.find(search));
}

