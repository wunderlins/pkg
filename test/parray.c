/**
 * dynamically growing pointer array
 * 
 * GOAL:
 * create a set of functions which will allocate a pointer array. if 
 * methods add items, the main array should grow to the desired size
 */

#include <stdlib.h>
#include <stdio.h>

#ifndef PARRAY_TESTCASE
#define PARRAY_TESTCASE 0
#endif

int _parray_errno = 0;

/**
 * This structure holds an array of pointers including the number of items
 */
typedef struct {
	size_t _psize;    // size of pointer
	size_t length;    // number of elements
	size_t allocated; // allocated memory
	size_t expand;    // number of elements to add if the array is full
	char** elements;  // actual data
} parray;

parray* parray_init(size_t psize, size_t expand);
size_t parray_set(parray* p, void* element, size_t pos); // if pos == 0, append
//size_t parray_free(parray* p);

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
size_t _parray_expand(parray* v) {
	_parray_errno = 0;
	size_t i = 0;
	
	// try to allocate more memory for data
	size_t s = v->_psize * (v->expand + v->allocated);
	v->elements = (char**) realloc(v->elements, s);
	
	// if realloc failed return 0
	if (v->elements == NULL) {
		_parray_errno = 1;
		return 0;
	}
	
	// set all newly allocated elements to NULL
	for (i=v->allocated; i<v->allocated + v->expand; i++) {
		v->elements[i] = NULL;
	}
	
	// make sure size matches the actual number of allocated array members
	v->allocated += v->expand;
	
	return v->allocated;
}

/**
 * Initialize
 *
 * Initialize memory for a strarray and return pointer to object. Will
 * return NULL upon error (usually because memory could not be allocated).
 *
 * @param psize
 * Size of the pointers stored in the array
 *
 * @param expand
 * number of elements which are pre allocated by _parray_expand
 *
 * @return parray* on success or NULL
 */
parray* parray_init(size_t psize, size_t expand) {
	parray* v = (parray*) malloc(sizeof(parray));
	
	// set attributes
	v->length = 0;
	v->expand = expand;
	v->allocated = 0;
	
	//_parray_expand(v);
	
	return v;
}

/**
 * Set an array value
 *
 * This method sets a value. If the element is past the current length, the 
 * empty elements will be added in between (with NULL value).
 *
 * @param v 
 * the parray to add the element to
 * 
 * @param element
 * any pointer to an object
 * 
 * @param pos
 * Position (array index) where to add. Might be past the array's length
 * 
 * @return size_t number of elements (length) in the array or 0 on error (usually mem allocation problem)
 */
size_t parray_set(parray* v, void* element, size_t pos) {
	_parray_errno = 0;
	
	while (pos >= v->allocated) {
		// extend array
		size_t r = _parray_expand(v);

		if (r == 0) {
			_parray_errno = 2;
			perror("parray_set failed _parray_expand");
			return 0;
		}
	}
	
	// set element
	v->elements[pos] = element;
	
	if (pos+1 > v->length)
		v->length = pos+1;
	
	return v->length;
}

#ifdef PARRAY_TESTCASE
#if PARRAY_TESTCASE > 0
int main() {
	
	parray* p = parray_init(sizeof(char*), 10);
	
	parray_set(p, "abc1", 0);
	parray_set(p, "abc2", 11);
	
	printf("length    %ld\nallocated %ld\nadd       %ld\n",
	       p->length, p->allocated, p->expand);
	
	size_t l = 0;
	for (l=0; l<p->allocated; l++)
		printf("%ld %s\n", l, p->elements[l]);
	
	return 0;
}
#endif
#endif
