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

size_t init_strvector(StrVector* v, size_t num_elements, size_t str_length);
size_t add_strvector(StrVector* v, char* element);
size_t _expand_strvector(StrVector* v);
void display_vector(StrVector* v);

size_t init_strvector(StrVector* v, size_t num_elements, size_t str_length) {
	v->count = 0;
	v->size = num_elements;
	v->_add = num_elements;
	v->str_length = str_length;
	
	// FIXME: allocate enough pointers and the allocate space for each array element in a loop
	v->data = malloc( sizeof(char) * (str_length+1) * num_elements );
	if (v->data == NULL)
		return 0;
	
	return num_elements;
}

size_t add_strvector(StrVector* v, char* element) {
	
	// check if the array is full
	if (v->count+1 > v->size) {
		// extend array
		size_t r = _expand_strvector(v);
		if (r == 0) {
			return 0;
		}
	}
	
	//v->data[v->count] = element;
	//printf("size: %lu %p\n", strlen(element), &(v->data[v->count]));
	memcpy(&(v->data[v->count]), &(element), strlen(element));
	v->count++;
	
	return v->count;
}

size_t _expand_strvector(StrVector* v) {
	v->data = realloc(v->data, sizeof(v->data) + sizeof(char) * (v->str_length+1) * v->_add);
	if (v->data == NULL)
		return 0;
	
	v->size += v->_add;
	
	return v->size;
}

void display_vector(StrVector* v) {
	size_t i;
	for (i=0; i<v->count; i++) {
    printf("%lu: %s\n", i, v->data[i]);
	}
}

int main(int argc, char** argv) {
	
	StrVector v;
	int r = init_strvector(&v, 10, 10);
	
	int i;
	char* str = "0123456789";
	for (i=0; i<10; i++) {
		add_strvector(&v, str);
	}
	//free(str);
	
	// printf("Test 1: %s\n", v.data[0]);
	display_vector(&v);
	
	printf("Vector size: %lu, count %lu\n", v.size, v.count);
	
	return 0;
}
