#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* name;
	char* value;
} n;

typedef struct {
	char* name;
	char* value;
	int count;
	void** children; // this is actually of type node_u, but it is declared further down
} nlist;

typedef union {
	n node;
	nlist list;
} node_u;

typedef struct {
	enum {
		ROOT,
		NODE,
		NODELIST
	} type;
	node_u* data;
} node_t;

node_t* init_node(char* name, char* value) {
	node_t* n = malloc(sizeof(node_t));
	n->data = malloc(sizeof(node_u));
	n->data->node.name = name;
	n->data->node.value = value;
	return n;
}

int main(int argc, char* argv[]) {
	
	node_t* n = malloc(sizeof(node_t));
	n->data = malloc(sizeof(node_u));
	n->data->node.name = "name";
	n->data->node.value = "val";
	
	printf("name: %s, value: %s\n", n->data->node.name, n->data->node.value);
	
	node_t* n1 = init_node("name1", NULL);
	printf("name: %s, value: %s\n", n1->data->node.name, n1->data->node.value);
	
	return 0;
}
