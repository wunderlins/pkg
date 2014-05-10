//============================================================================
// Name        : stringlib.h
// Author      : Simon Wunderlin
// Version     : $Id: stringlib.h 63 2011-04-16 10:35:25Z wus $
// Copyright   : GPL2
// Description : string manipulation collection
//============================================================================

#ifndef STRINGLIB_H
#define STRINGLIB_H

/** 
 * Find the 1st occurance of '=' in string
 */
int first_index_of(char, std::string);

/** 
 * convert string to char array
 */
char * string2char(std::string);

/**
 * convert int to char array
 */
void int2char(int, char*);

/**
 * left trim
 *
 * remove all spaces at the beginning of a string
 */
std::string ltrim(std::string);

#endif
