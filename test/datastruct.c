#include <stdio.h>
#include <stdlib.h>

/*
// simple structure
typedef struct {
	int i;
	float f;
} s1;

//struct with pointer
typedef struct {
	int i;
	char* name;
} s2;

// struct with pointer pointer
typedef struct {
	char* name;
	int length;
	s1* items[];
} s3;

// struct with pointer array with complex struct
typedef struct {
	char* name;
	int length;
	s3** items;
} s4;

void dump_s1(s1* st) {
	printf("i: %d, f: %f\n", st->i, st->f);
}

void dump_s2(s2* st) {
	printf("i: %d, name: %s\n", st->i, st->name);
}

void dump_s3(s3* st) {
	printf("length: %d, name: %s\n", st->length, st->name);
	int i = 0;
	
	for (i=0; i < st->length; i++) {
		printf("--> %d, ", i);
		dump_s1( (*st->items + i) );
	}
	
}
*/

typedef enum {
	ROOT,          // 0
	NODE,          // 1 
	NODELIST
} nodetype_t;

typedef struct {
	nodetype_t type;
	int length;
	void** nodes;
} root_t;

typedef struct {
	nodetype_t type;
	char* name;
	char* value;
} node_t;

typedef struct {
	nodetype_t type;
	int length;
	char* name;
	char* value;
	void** nodes;
} nodelist_t;

root_t init_root();
node_t init_node(char* name, char* value);
nodelist_t init_nodelist(char* name, char* value);

node_t init_node(char* name, char* value) {
	node_t n = {
		.type  = NODE,
		.name  = name,
		.value = value
	};
	return n;
}

nodelist_t init_nodelist(char* name, char* value) {
	nodelist_t n = {
		.type  = NODELIST,
		.length = 0,
		.name  = name,
		.value = value,
		.nodes = NULL
	};
	return n;
}

int main() {
	
	/*
	// allocate mem for simple struct
	s1* i1 = (s1*) malloc(sizeof(s1));
	i1->i = 10;
	i1->f = 3.142;
	dump_s1(i1);
	
	// allocate mem for struct with pointer
	s2* i2 = (s2*) malloc(sizeof(s2));
	i2->i = 10;
	i2->name = "the name";
	dump_s2(i2);
	
	// alloc pointer pointer
	s3* i3 = (s3*) malloc(sizeof(s3));
	i3->name = "recordset";
	i3->length = 0;
	i3->items[i3->length++] = i1;
	s1* i11 = (s1*) malloc(sizeof(s1));
	i11->i = 5;
	i11->f = 2.334;
	i3->items[i3->length++] = i11;
	//i3->items++;
	dump_s3(i3);
	*/
	
	node_t n0 = init_node("Host", "localhost");
	node_t n1 = init_node("Port", "8080");
	
	printf("%s = %s\n", n0.name, n0.value);
	
	return 0;
}
