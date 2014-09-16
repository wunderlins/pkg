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
 * TODO: add set(num) method
 * TODO: add remove(num) method
 * TODO: add get_length method
 * TODO: add find method
 * TODO: add possibility to ad by reference
 *
 * (c) 2014, Simon Wunderlin <swunderlin()gmailDTcom>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This structure holds an array of strings including the number of items, 
 * allocated memory and length of string elements
 */
typedef struct {
	size_t count;      // number of elements
	size_t size;       // number of possible elements
	size_t _add;       // number of elements to add if the array is full
	size_t str_length; // length of a single array element
	char** elements;   // actual data
} StringArray;

/**
 * Initialize
 *
 * Initialize memory for a StringArray and return pointer to object. Will
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
 * @return StringArray* on success or NULL
 */
StringArray* stringarray_init(size_t num_elements, size_t str_length);

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
 * FIXME: check the size of the input string and make sure it fits into the target
 *
 * @param v 
 * the string array to add the element to
 * 
 * @param element
 * A string that should not be larger than the internal string length
 * 
 * @return size_t number of elements (length) in the array or 0 on error (usually mem allocation problem)
 */
size_t stringarray_add(StringArray* v, char* element);

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
size_t _stringarray_expand(StringArray* v);

/**
 * Free data
 *
 * Use this befrore free-ing the struct itself.
 *
 * @param v 
 * the string array to add the element to
 */
void stringarray_free(StringArray* v);

/**
 * display the contents of the array line by line
 *
 * @param v 
 * the string array to add the element to
 */
void stringarray_display(StringArray* v);

/**
 * Initialize
 *
 * Initialize memory for a StringArray and return pointer to object. Will
 * return NULL upon error (usually because memory could not be allocated).
 *
 * FIXME: might want to use errno upon error?
 *
 * @param num_elements
 * This is the number of array elements we are allocating memory for
 *
 * @param str_length
 * The maximum length of a string (excluding the \0 delimiter)
 *
 * @return StringArray* on success or NULL
 */
StringArray* stringarray_init(size_t num_elements, size_t str_length) {
	
	// allocate memory for the struct that we will return
	StringArray* v = malloc(sizeof(StringArray));
	
	// set attributes
	v->count = 0;
	v->size = num_elements;
	v->_add = num_elements;
	v->str_length = str_length;
	
	// allocate enough pointers for the strings 
	v->elements = malloc( sizeof(char*) * v->size );
	if (v->elements == NULL)
		return NULL;
	
	// allocate space for each array element
	int i;
	for (i=0; i<v->size; i++) {
		v->elements[i] = malloc(sizeof(char) * (v->str_length + 1));
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
 * FIXME: check the size of the input string and make sure it fits into the target when adding an elements
 *
 * @param v 
 * the string array to add the element to
 * 
 * @param element
 * A string that should not be larger than the internal string length
 * 
 * @return size_t number of elements (length) in the array or 0 on error (usually mem allocation problem)
 */
size_t stringarray_add(StringArray* v, char* element) {
	
	// check if the array is full
	if (v->count+1 > v->size) {
		// extend array
		size_t r = _stringarray_expand(v);
		if (r == 0)
			return 0;
	}
	
	// FIXME: probably we need to handle NULL pointers in some meaningful way
	
	// copy string into pre-allocated memory inside this object
	// FIXME: check that every elements is copied into our own memory location!
	memcpy(v->elements[v->count], element, sizeof(char) * v->str_length+1);
	
	// remeber the new element count
	v->count++;
	
	return v->count;
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
size_t _stringarray_expand(StringArray* v) {
	
	// try to allocate more memory for data		
	v->elements = realloc(v->elements, sizeof(char*) * (v->_add + v->size));
	
	// if it failed return 0
	if (v->elements == NULL)
		return 0;
	
	// pre allocate memory for the ne members
	size_t i;
	for (i=v->size; i<v->size+v->_add; i++) {
		v->elements[i] = malloc(sizeof(char) * (v->str_length + 1));
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
 * Use this befrore free-ing the struct itself.
 *
 * @param v 
 * the string array to add the element to
 */
void stringarray_free(StringArray* v) {
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
void stringarray_display(StringArray* v) {
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
	
	// allocate StringArray
	StringArray* v = stringarray_init(10, 15);
	
	// populate the array with dummy data
	int i;
	char* str = malloc(sizeof(char)*16);
	memcpy(str, "0123456789000--", 16);
	for (i=0; i<11; i++) {
		stringarray_add(v, str);
	}
	
	// free the original string
	free(str);
	
	// display all array elements fro mthe array
	printf("Memory location of StringArray: %p\n", v);
	stringarray_display(v);
	printf("Array dimensions: %lu, count %lu\n", v->size, v->count);
	
	// release the memory, destroy the object and it's data
	stringarray_free(v);
	free(v);
	
	return 0;
}

