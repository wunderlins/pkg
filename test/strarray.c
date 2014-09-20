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
 * TODO: variable length array or allocate all memory
 * TODO: add get_length method
 * TODO: add find method
 *
 * (c) 2014, Simon Wunderlin <swunderlin()gmailDTcom>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strarray.h"

#define STRARRAY_DEBUG 0

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
	_strarray_errno = 0;
	
	// try to allocate more memory for data		
	v->elements = realloc(v->elements, sizeof(char*) * (v->_add + v->_memsize));
	
	// if it failed return 0
	if (v->elements == NULL) {
		_strarray_errno = 1;
		return 0;
	}
	
	// pre allocate memory for the new members
	size_t i;
	for (i=v->_memsize; i<v->_memsize+v->_add; i++) {
		v->elements[i] = malloc(sizeof(char) * (v->str_length + 1));
		//printf("idx %d\n", i);
		if (v->elements[i] == NULL) {
			_strarray_errno = 1;
			return 0;
		}
		// set initial value to empty string
		v->elements[i][0] = '\0';
	}
	
	// make sure size matches the actual number of allocated array members
	v->_memsize += v->_add;
	
	return v->_memsize;
}

/**
 * Initialize
 *
 * Initialize memory for a strarray and return pointer to object. Will
 * return NULL upon error (usually because memory could not be allocated).
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
	_strarray_errno = 0;
	
	// allocate memory for the struct that we will return
	StrArray* v = malloc(sizeof(StrArray));
	
	// set attributes
	v->length = 0;
	v->_memsize = 0; // set to num_elements after calling _strarray_expand
	v->_add = num_elements;
	v->str_length = str_length;
	
	// allocate enough pointers for the strings 
	v->elements = malloc( sizeof(char*) * v->_memsize );
	if (v->elements == NULL) {
		_strarray_errno = 1;
		return NULL;
	}
	
	// allocate mem for the first batch of strings
	size_t r = _strarray_expand(v);
	if (r == 0) {
		_strarray_errno = 2;
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
	_strarray_errno = 0;
	size_t r = strarray_set(v, element, v->length);
	return v->length;
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
	_strarray_errno = 0;
	
	while (pos >= v->_memsize) {
		// extend array
		size_t r = _strarray_expand(v);
		if (r == 0) {
			_strarray_errno = 2;
			return 0;
		}
	}
	
	// check the size of the input string and make sure it fits into the target
	// when adding an elements
	if (strlen(element) > v->str_length) {
		_strarray_errno = 3;
		return 0;
	}
	
	// copy string into pre-allocated memory inside this object
	memcpy(v->elements[pos], element, sizeof(char) * v->str_length+1);
	
	//printf("Added\n");
	
	if (pos+1 > v->length)
		v->length = pos+1;
	
	return v->length;
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
	_strarray_errno = 0;
	
	size_t i = 0;
	// check if element exists
	if (pos >= v->length) {
		_strarray_errno = 4;
		return 0;
	}
	
	char* removed = v->elements[pos];
	
	// move all subsequent elements to the front by one
	for(i=pos; i < v->length-1; i++)
		v->elements[i] = v->elements[i+1];

	v->elements[v->length-1] = removed;
	return --(v->length);
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
	for (i=0; i<v->length; i++) {
		printf("%lu %p: %s\n", i, v->elements[i], v->elements[i]);
	}
}

/**
 * checks if there was an error
 * 
 * @return 1 on success, 0 if there is an unhandled error
 */
int strarray_error() {
	return (_strarray_errno) ? 0 : 1;
}

/**
 * get the error description
 * 
 * @return error description
 */
const char* strarray_errstr() {
	_strarray_errno = 0;
	
	if (_strarray_errno+1 > STRARRAY_ERR_MAX) {
		_strarray_errno = 5;
		return NULL;
	}
	
	return _strarray_errstr[_strarray_errno];
}

/**
 * Example usage
 */
int main(int argc, char** argv) {
	int res = 0;
	// allocate strarray
	StrArray* v = strarray_init(1, 15);
	if (v == NULL) {
		fprintf(stdout, "Error: %d %s\n", _strarray_errno, strarray_errstr());
		return 1;
	}
	//printf("after init\n");
	
	// populate the array with dummy data
	int i;
	char* str = malloc(sizeof(char)*16);
	memcpy(str, "0123456789000--", 16);
	for (i=0; i<12; i++) {
		//printf("copy element: %d\n", i);
		res = strarray_add(v, str);
		if (res == 0) {
			fprintf(stdout, "Error: %d %s\n", _strarray_errno, strarray_errstr());
			return 1;
		}
	}
	//printf("done allocating\n");
	
	res = strarray_set(v, "12345", 19);
	if (res == 0) {
		fprintf(stdout, "Error: %d %s\n", _strarray_errno, strarray_errstr());
		return 1;
	}
	
	res = strarray_set(v, "12345", 4);
	if (res == 0) {
		fprintf(stdout, "Error: %d %s\n", _strarray_errno, strarray_errstr());
		return 1;
	}
	
	res = strarray_set(v, "12345", 13);
	if (res == 0) {
		fprintf(stdout, "Error: %d %s\n", _strarray_errno, strarray_errstr());
		return 1;
	}
		
	// free the original string
	free(str);
	
	//strarray_display(v);
	
	res = strarray_remove(v, 12);
	if (res == 0) {
		fprintf(stdout, "Error: %d %s\n", _strarray_errno, strarray_errstr());
		return 1;
	}
	
	res = strarray_remove(v, 8);
	if (res == 0) {
		fprintf(stdout, "Error: %d %s\n", _strarray_errno, strarray_errstr());
		return 1;
	}
	
	// display all array elements from the array
	printf("Memory location of strarray: %p\n", v);
	strarray_display(v);
	printf("Array dimensions: %lu, count %lu\n", v->_memsize, v->length);
	
	// release the memory, destroy the object and it's data
	strarray_free(v);
	free(v);
	
	return 0;
}
