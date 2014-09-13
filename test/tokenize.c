//
//  tokenize.c
//  pkg
//
//  Created by Simon Wunderlin on 13/09/14.
//  Copyright (c) 2014 ___HACKERSINC___. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#define NUM_TOKENS 3
typedef char tokens[NUM_TOKENS][80];

typedef struct {
	const int max_tokens;
	const int mac_char;
	int items;
	tokens* t;
} tokens_t;

/**
 * Split string by susequent spaces
 *
 * returns the number of elements
 */
int tokenize(tokens result, char* string) {
	
	char last;
	int space_count, i, word = 0;
	int length = strlen(string);
	char buffer[100] = "";
	int buffer_l = 0;
	
	// loop over the array character by character
	for (i=0; i<length; i++) {
		last = string[i];
		
		if (last == ' ')
			space_count++;
		
		if (space_count > 1) {
			buffer[buffer_l-1] = '\0'; // remove last space from buffer
			strcpy(result[word++], buffer);
			//printf("--> %s\n", buffer);
			
			// clear buffer
			buffer[0] = '\0';
			buffer_l = 0;
			
			// are we done?
			if (word == NUM_TOKENS) {
				break;
			}
			
			// find next non space character
			for (; string[i] == ' '; i++);
			space_count = 0;
			i--;
			continue;
		}
		
		// remeber character
		buffer[buffer_l++] = last;
		buffer[buffer_l] = '\0';
	}
	
	// copy last buffer into the result
	if (buffer[0] != '\0') {
		strcpy(result[word++], buffer);
		word++;
	}
	
	return word-1;
}

int main() {
	
	char *string = "module-name      @1.2.sdef       mod/submod";
	tokens result;
	
	int count = tokenize(result, string);
	
	printf("%d\n", count);
	printf("%s\n", result[0]);
	printf("%s\n", result[1]);
	printf("%s\n", result[2]);
	
	return 0;
}