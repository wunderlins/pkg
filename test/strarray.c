/**
 * Dynamic string array
 * 
 * These methods provide a mechanism to store a variable number of char 
 * arrays (strings) in a struct. All added strings are copied and thus 
 * not dependend on the original memory location of a string. These methods 
 * are not optimized for memory usage but for ease of use on small string 
 * arrays.
 *
 * Using the provided mechanism will ensure that there is always enough memory 
 * allocated to store new members of the string array. lenght of strings is
 * fixed and set upon initialisation.
 *
 * TODO: add get_length method
 * TODO: add find method
 * TODO: add possibility to ad by reference
 *
 * (c) 2014, Simon Wunderlin <swunderlin()gmailDTcom>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strarray.h"

/**
 * Initialize
 *
 * Initialize memory for a strarray and return pointer to object. Will
 * return NULL upon error (usually because memory could not be allocated).
 *
 * TODO: might want to use errno upon error?
 *
 * @param num_elements
 * This is the number of array elements we are allocating memory for
 *
 * @param str_length
 * The maximum length of a string (excluding the \0 delimiter)
 *
 * @return strarray* on success or NULL
 */
StrArray* strarray_init(size_t num_elements, size_t str_length) {
	
	// allocate memory for the struct that we will return
	StrArray* v = malloc(sizeof(StrArray));
	
	// set attributes
	v->count = 0;
	v->size = num_elements;
	v->_add = num_elements;
	v->str_length = str_length;
	v->_null = _strarray_null;
	
	// allocate enough pointers for the strings 
	v->elements = malloc( sizeof(char*) * v->size );
	if (v->elements == NULL)
		return NULL;
	
	// allocate space for each array element
	int i;
	for (i=0; i<v->size; i++) {
		v->elements[i] = (char*) v->_null; // malloc(sizeof(char) * (v->str_length + 1));
		if (v->elements[i] == NULL)
			return NULL;
	}
	
	return v;
}

/**
 * Add string to the array
 *
 * This function will add a string at the end of the array. It will also 
 * automatically enlarge ->elements if required. The value use to enlarge the 
 * data storage is as large as the initial num_elements.
 *
 * You must check that the length of element does not exceed the 
 * internal str_length.
 * 
 * @param v 
 * the string array to add the element to
 * 
 * @param element
 * A string that should not be larger than the internal string length
 * 
 * @return size_t number of elements (length) in the array or 0 on error (usually mem allocation problem)
 */
size_t strarray_add(StrArray* v, char* element) {
	
	// check if the array is full
	if (v->count+1 > v->size) {
		// extend array
		size_t r = _strarray_expand(v);
		if (r == 0)
			return 0;
	}
	
	// if no memory is allocated, do this now
	if (v->elements[v->count] == v->_null || v->elements[v->count] == NULL) {
		//free(v->elements[v->count]);
		v->elements[v->count] = malloc(sizeof(char)*(v->str_length+1));
	}

	// check the size of the input string and make sure it fits into the target
	// when adding an elements
	if (strlen(element) > v->str_length)
		return 0;

	// copy string into pre-allocated memory inside this object
	memcpy(v->elements[v->count], element, sizeof(char) * v->str_length+1);
	
	// remeber the new element count
	v->count++;
	
	return v->count;
}

/**
 * Set an array value
 *
 * This method sets a value. If the element is past the current length, the 
 * empty elements will be added inbetween.
 *
 * @param v 
 * the string array to add the element to
 * 
 * @param element
 * A string that should not be larger than the internal string length
 * 
 * @param pos
 * Position (array index) where to add. Might be past the array's length
 * 
 * @return size_t number of elements (length) in the array or 0 on error (usually mem allocation problem)
 */
size_t strarray_set(StrArray* v, char* element, size_t pos) {
	
	while (pos > v->size) {
		// extend array
		size_t r = _strarray_expand(v);
		if (r == 0)
			return 0;
	}
	
	//printf("Expanded\n");
	
	// if no memory is allocated, do this now
	if (v->elements[pos] == v->_null || v->elements[pos] == NULL) {
		//free(v->elements[v->count]);
		v->elements[pos] = malloc(sizeof(char)*(strlen(element)+1));
	}
	
	// copy string into pre-allocated memory inside this object
	memcpy(v->elements[pos], element, sizeof(char) * v->str_length+1);
	
	//printf("Added\n");
	
	if (pos+1 > v->count)
		v->count = pos+1;
	
	return v->count;
}

/**
 * remove element from array
 *
 * @param v
 * the string array to add the element to
 *
 * @param pos
 * Numeric position of element to be removed
 *
 * @return size_t number of elements (length) in the array or 0 on error (usually mem allocation problem)
 */
size_t strarray_remove(StrArray* v, size_t pos) {
	size_t i = 0;
	// check if element exists
	if (pos >= v->count)
		return 0;
	
	char* removed = v->elements[pos];
	
	// move all subsequent elements to the front by one
	for(i=pos; i < v->count-1; i++)
		v->elements[i] = v->elements[i+1];

	v->elements[v->count-1] = removed;
	return --(v->count);
}

/**
 * Make storage larger
 *
 * This function will automatically enlarge ->elements if required. The value use
 * to enlarge the data storage is as large as the initial num_elements.
 *
 * @param v 
 * the string array to add the element to
 * 
 * @return size_t number allocated elements in the array or 0 on error (usually mem allocation problem)
 */
size_t _strarray_expand(StrArray* v) {
	
	// try to allocate more memory for data		
	v->elements = realloc(v->elements, sizeof(char*) * (v->_add + v->size));
	
	// if it failed return 0
	if (v->elements == NULL)
		return 0;
	
	// pre allocate memory for the ne members
	size_t i;
	for (i=v->size; i<v->size+v->_add; i++) {
		v->elements[i] = (char*) v->_null; // malloc(sizeof(char) * (v->str_length + 1));
		if (v->elements[i] == NULL)
			return 0;
	}
	
	// make sure size matches the actual number of allocated array members
	v->size += v->_add;
	
	return v->size;
}

/**
 * Free data
 *
 * Use this before free-ing the struct itself.
 *
 * @param v 
 * the string array to add the element to
 */
void strarray_free(StrArray* v) {
	// free the elements array, after this call no elements will be available
	free(v->elements);
}

/**
 * display the contents of the array line by line
 *
 * This function is mostly used for debugging.
 *
 * @param v 
 * the string array to add the element to
 */
void strarray_display(StrArray* v) {
	// loop over all set elements and display them.
	size_t i;
	for (i=0; i<v->count; i++) {
		printf("%lu %p: %s\n", i, v->elements[i], v->elements[i]);
	}
}

/**
 * Example usage
 */
int main(int argc, char** argv) {
	
	// allocate strarray
	StrArray* v = strarray_init(10, 15);
	
	// populate the array with dummy data
	int i;
	char* str = malloc(sizeof(char)*16);
	memcpy(str, "0123456789000--", 16);
	for (i=0; i<12; i++) {
		strarray_add(v, str);
	}
	
	strarray_set(v, "12345", 19);
	strarray_set(v, "12345", 4);
	strarray_set(v, "12345", 13);
	
	// free the original string
	free(str);
	
	//strarray_display(v);

	strarray_remove(v, 12);
	strarray_remove(v, 8);

	// display all array elements fro mthe array
	printf("Memory location of strarray: %p\n", v);
	strarray_display(v);
	printf("Array dimensions: %lu, count %lu\n", v->size, v->count);
	
	// release the memory, destroy the object and it's data
	strarray_free(v);
	free(v);
	
	return 0;
}

