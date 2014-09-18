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
 * (c) 2014, Simon Wunderlin <swunderlin()gmailDTcom>
 */

/**
 * lame attempt to define a null value for an array element
 */
const char* _strarray_null = "";
int _strarray_errno = 0; // global variables are always extern?
#define STRARRAY_ERR_MAX 6
#define STRARRAY_ERR_LENGTH 50
const char _strarray_errstr[STRARRAY_ERR_MAX][STRARRAY_ERR_LENGTH] = {
		/*  0 */ "",
		/*  1 */ "Failed to allocate memory",
		/*  2 */ "_strarray_expand failed",
		/*  3 */ "Internal string length too short for input",
		/*  4 */ "remove failed, index larger than array size",
		/*  5 */ "Unknown error number",
};

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
	const char* _null;
} StrArray;

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
StrArray* strarray_init(size_t num_elements, size_t str_length);

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
size_t strarray_add(StrArray* v, char* element);

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
size_t strarray_set(StrArray* v, char* element, size_t pos);

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
size_t strarray_remove(StrArray* v, size_t pos);

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
size_t _strarray_expand(StrArray* v);

/**
 * Free data
 *
 * Use this before free-ing the struct itself.
 *
 * @param v
 * the string array to add the element to
 */
void strarray_free(StrArray* v);

/**
 * checks if there was an error
 * 
 * @return 1 on success, 0 if there is an unhandled error
 */
int strarray_error();

/**
 * get the error description
 * 
 * @return error description
 */
const char* strarray_errstr();

/**
 * display the contents of the array line by line
 *
 * This function is mostly used for debugging.
 *
 * @param v
 * the string array to add the element to
 */
void strarray_display(StrArray* v);
