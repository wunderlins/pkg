//
//  tokenize.c
//  pkg
//
//  Created by Simon Wunderlin on 13/09/14.
//  Copyright (c) 2014 ___HACKERSINC___. All rights reserved.
//

#include <stdio.h>
#include <string.h>

typedef char tokens[3][80];

/**
 * Split string by susequent spaces
 *
 * returns the number of elements
 */
int tokenize(tokens result, char* string) {
	
	char last;
	int space_count, i = 0;
	int length = strlen(string);
	char buffer[100] = "";
	int buffer_l = 0;
	
	// loop over the array character by character
	for (i=0; i<length; i++) {
		last = string[i];
		
		if (last == ' ')
			space_count++;
		
		if (space_count > 1) {
			// TODO: next token
		}
		
		buffer[buffer_l++] = last;
	}
	
	return 0;
}

int main() {
	
	char *string = "module-name      @1.2.sdef       mod/submod";
	tokens result;
	
	tokenize(result, string);
	
	return 0;
}