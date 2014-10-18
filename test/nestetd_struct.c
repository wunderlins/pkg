#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* name;
	char* value;
} payload;

typedef union {
	payload p;
} node_u;

typedef struct {
	enum {
		ROOT,
		NODE,
		NODELIST
	} type;
	node_u* data;
} node_t;

node_t* init_node() {
	node_t* n = malloc(sizeof(node_t));
	n->data = malloc(sizeof(node_u));
	n->data->p.name = "name";
	n->data->p.value = "val";
	return n;
}

int main(int argc, char* argv[]) {
	
	node_t* n = malloc(sizeof(node_t));
	n->data = malloc(sizeof(node_u));
	n->data->p.name = "name";
	n->data->p.value = "val";
	
	printf("name: %s, value: %s\n", n->data->p.name, n->data->p.value);
	
	node_t* n1 = init_node();
	printf("name: %s, value: %s\n", n1->data->p.name, n1->data->p.value);
	
	return 0;
}
