#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char* name;
	char* value;
} n;

typedef struct {
	char* name;
	char* value;
	int length;
	void** children; // this is actually of type node_u, but it is declared further down
} nlist;

typedef union {
	n node;
	nlist list;
} node_u;

typedef struct {
	enum {
		UNDEF,   // 0
		ROOT,    // 1
		NODE,    // 2
		NODELIST // 3
	} type;
	node_u* data;
} node_t;

node_t* init_node(char* name, char* value) {
	node_t* n = malloc(sizeof(node_t));
	n->type = NODE;
	n->data = malloc(sizeof(node_u));
	n->data->node.name = name;
	n->data->node.value = value;
	return n;
}

node_t* init_nodelist(char* name, char* value) {
	node_t* n = malloc(sizeof(node_t));
	n->type = NODELIST;
	n->data = malloc(sizeof(node_u));
	n->data->list.name = name;
	n->data->list.value = value;
	n->data->list.length = 0;
	n->data->list.children = NULL;
	return n;
}

int node_append(node_t* nodelist, node_t* node) {
	
	// check if nodelist accepts children
	if (nodelist->type != NODELIST)
		return 1;
	
	// allocate memory for children
	if (nodelist->data->list.length == 0) {
		// malloc
		nodelist->data->list.children = malloc(sizeof(node_t));
		if (nodelist->data->list.children == NULL)
			return 2;
	} else {
		// realloc
		nodelist->data->list.children = 
				realloc(nodelist->data->list.children, 
				sizeof(node_t) * nodelist->data->list.length);
		if (nodelist->data->list.children == NULL)
			return 2;
	}
	
	node_t** children = (node_t**) nodelist->data->list.children;
	children[nodelist->data->list.length] = node;
	nodelist->data->list.length++;
	
	return 0; // success
}

int node_to_str(char* buffer, node_t* node) {
	switch(node->type) {
		case UNDEF:
			strcpy(buffer, "Type: UNDEF");
			break;
		
		case ROOT:
			strcpy(buffer, "Type: ROOT");
			break;
		
		case NODE:
			printf(" %s ", node->data->node.name);
			strcpy(buffer, "Type: NODE     ");
			strcat(buffer, "name: ");
			strcat(buffer, (const char*) node->data->node.name);
			strcat(buffer, ", value: ");
			strcat(buffer, (const char*) node->data->node.value);
			break;
		
		case NODELIST:
			strcpy(buffer, "Type: NODELIST ");
			strcat(buffer, "name: ");
			strcat(buffer, node->data->list.name);
			strcat(buffer, ", value: ");
			strcat(buffer, node->data->list.value);
			strcat(buffer, ", length: ");
			char l[20];
			sprintf(l, "%d", node->data->list.length);
			strcat(buffer, l);
			break;
	}
	
	return 0;
}


int main(int argc, char* argv[]) {
	
	node_t* n = malloc(sizeof(node_t));
	n->data = malloc(sizeof(node_u));
	n->data->node.name = "name";
	n->data->node.value = "val";
	
	printf("%p %d: name: %s, value: %s\n", n, n->type, n->data->node.name, n->data->node.value);
	
	node_t* n1 = init_node("name1", NULL);
	printf("%p %d: name: %s, value: %s\n", n1, n1->type, n1->data->node.name, n1->data->node.value);
	
	node_t* nl1 = init_nodelist("nl1", "val1");
	printf("%p %d: name: %s, value: %s\n", nl1, nl1->type, nl1->data->list.name, nl1->data->list.value);
	
	node_append(nl1, n1);
	node_t** children = (node_t**) nl1->data->list.children;
	//nl1->data->list.children[0]->data.node->name
	printf("%p %d: length: %d, type: %d, name: %s, value: %s\n", 
			                                 nl1,
			                                 nl1->type, 
			                                 nl1->data->list.length, 
			                                 children[0]->type,
			                                 children[0]->data->node.name,
			                                 children[0]->data->node.value);
	node_t* child = children[0];
	printf("%p %d: length: %d, type: %d, name: %s, value: %s\n", 
			                                 nl1, 
			                                 nl1->type, 
			                                 nl1->data->list.length, 
			                                 child->type,
			                                 child->data->node.name,
			                                 child->data->node.value);
	
	printf("%p %d: name: %s, value: %s\n", 
	                                    child,
	                                    child->type, 
	                                    child->data->node.name, 
	                                    child->data->node.value);
	
	char nstr[100] = "";
	node_to_str(nstr, n1);
	printf("%p %s\n", n1, nstr);
	
	return 0;
}
