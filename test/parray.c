/**
 * dynamically growing pointer array
 * 
 * GOAL:
 * create a set of functions which will allocate a pointer array. if 
 * methods add items, the main array should grow to the desired size
 */

#define PARRAY_TESTCASE 0

/**
 * This structure holds an array of pointers including the number of items
 */
typedef struct {
	size_t length;    // number of elements
	size_t memsize;   // number of possible elements
	size_t add;       // number of elements to add if the array is full
	char** elements;  // actual data
} parray;

parray* parray_init();
size_t parray_set(parray* p, size_t pos);
size_t parray_add(parray* p, size_t pos); // do we need add if we have set?
size_t parray_shrink(parray* p, size_t pos);
size_t parray_free(parray* p);


#ifdef PARRAY_TESTCASE
#if PARRAY_TESTCASE > 0
int main() {
	
	return 0;
}
#endif
#endif
