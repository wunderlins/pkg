#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	size_t count; // number of elements
	size_t size;  // number of possible elements
	size_t _add;  // number of elements to add if the array is full
	size_t str_length; // length of a single array element
	char** data;  // actual data memory
} StrVector;

StrVector* init_strvector(size_t num_elements, size_t str_length);
size_t add_strvector(StrVector* v, char* element);
size_t _expand_strvector(StrVector* v);
void display_vector(StrVector* v);
void free_strvector(StrVector* v);

StrVector* init_strvector(size_t num_elements, size_t str_length) {

	StrVector* v = malloc(sizeof(StrVector));
	
	v->count = 0;
	v->size = num_elements;
	v->_add = num_elements;
	v->str_length = str_length;
	
	// FIXME: allocate enough pointers and the allocate space for each array element in a loop
	v->data = malloc( sizeof(char*) * v->size );
	if (v->data == NULL)
		return NULL;
	
	int i;
	for (i=0; i<v->size; i++) {
    v->data[i] = malloc(sizeof(char) * (v->str_length + 1));
		if (v->data[i] == NULL)
			return NULL;
	}
	
	return v;
}

size_t add_strvector(StrVector* v, char* element) {
	
	// check if the array is full
	if (v->count+1 > v->size) {
		// extend array
		size_t r = _expand_strvector(v);
		if (r == 0)
			return 0;
	}
	
	//v->data[v->count] = element;
	//printf("size: %lu %p %lu\n", sizeof(char) * v->str_length+1, &v->data[v->count], v->count);
	memcpy((v->data[v->count]), element, sizeof(char) * v->str_length+1);
	(v->count)++;
	
	return v->count;
}

size_t _expand_strvector(StrVector* v) {
	v->data = realloc(v->data, sizeof(char*) * (v->_add + v->size));
	if (v->data == NULL)
		return 0;
	
	size_t i;
	for (i=v->size; i<v->size+v->_add; i++) {
		//printf("%zu\n", i);
    v->data[i] = malloc(sizeof(char) * (v->str_length + 1));
		if (v->data[i] == NULL)
			return 0;
	}
	
	v->size += v->_add;
	
	return v->size;
}

void free_strvector(StrVector* v) {
	free(v->data);
}

void display_vector(StrVector* v) {
	size_t i;
	for (i=0; i<v->count; i++) {
    printf("%lu: %s\n", i, v->data[i]);
	}
}

int main(int argc, char** argv) {
	
	StrVector* v = init_strvector(10, 15);
	
	int i;
	char* str = "0123456789000--";
	for (i=0; i<10; i++) {
		add_strvector(v, str);
	}
	
	//v.data[11] = "-012345678901234";
	//v.count = 12;
	
	char* str2 = malloc(sizeof(char) * 5);
	free(str2);
	
	// printf("Test 1: %s\n", v.data[0]);
	display_vector(v);
	
	printf("Vector size: %lu, count %lu\n", v->size, v->count);
	free_strvector(v);
	
	//display_vector(&v);
	
	return 0;
}
