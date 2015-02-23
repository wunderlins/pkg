#!/usr/bin/env C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	size_t count;
	size_t length;
	size_t _add;
	size_t string_length;
	char** data;
} struct_t;

struct_t* init(size_t count, size_t string_length) {
	struct_t* s = malloc(sizeof(struct_t));
	
	printf("%lu, %lu\n", sizeof(s), sizeof(struct_t *));
	
	s->count = count;
	s->length = 0;
	s->_add = count;
	s->string_length = string_length;
	
	printf("Allocating array\n");
	s->data = malloc(sizeof(char*) * s->count);
	
	size_t i;
	for(i=0; i < s->count; i++) {
		printf("Allocating element %lu\n", i);
		s->data[i] = malloc(sizeof(char) * (string_length+1));
	}
	
	return s;
}

int main() {
	
	struct_t* s = init(10, 80);
	int i = 0;
	
	for(i=0; i<10; i++) {
		char* str = malloc(sizeof(char)*(80+1));
		sprintf(str, "%d asdf asdf asdf asdf ", i);
		memcpy(s->data[i], str, 80+1);
		free(str);
	}
	
	printf("%s\n", s->data[0]);
	printf("End of struct.\n");
	return 0;
}
