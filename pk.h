#ifndef _PK_H_
#define _PK_H_

#define PK_MODES_LENGTH 7

typedef enum {
	/* 0 */ NONE, 
	/* 1 */ META, 
	/* 2 */ UPDATEABLE, 
	/* 3 */ UPDATE, 
	/* 4 */ LIST, 
	/* 5 */ SEARCH, 
	/* 6 */ INSTALL
} modes_t;

#endif
