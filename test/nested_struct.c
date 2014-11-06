#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int nodeid = 0;

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

typedef struct {
	int length;
	void** children; // this is actually of type node_u, but it is declared further down
} rlist;

typedef union {
	n node;
	nlist list;
} node_u;

typedef struct {
	int nodeid;
	int has_children;
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
	n->nodeid = nodeid++;
	n->has_children = 0;
	n->type = NODE;
	n->data = malloc(sizeof(node_u));
	n->data->node.name = name;
	n->data->node.value = value;
	return n;
}

node_t* init_nodelist(char* name, char* value) {
	node_t* n = malloc(sizeof(node_t));
	n->nodeid = nodeid++;
	n->has_children = 1;
	n->type = NODELIST;
	n->data = malloc(sizeof(node_u));
	n->data->list.name = name;
	n->data->list.value = value;
	n->data->list.length = 0;
	n->data->list.children = NULL;
	return n;
}

node_t* init_root() {
	node_t* n = malloc(sizeof(node_t));
	n->nodeid = nodeid++;
	n->has_children = 1;
	n->type = ROOT;
	n->data = malloc(sizeof(node_u));
	n->data->list.length = 0;
	n->data->list.children = NULL;
	return n;
}

int node_append(node_t* nodelist, node_t* node) {
	
	// check if nodelist accepts children
	if (nodelist->type != NODELIST && nodelist->type != ROOT)
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

char* node_to_str(char* buffer, node_t* node) {
	//char* buffer = malloc(sizeof(char)*101);
	buffer[0] = '\0';
	char l[20];
	sprintf(buffer, "%p %d ", node, node->nodeid);
	
	switch(node->type) {
		case UNDEF:
			strcat(buffer, "UNDEF");
			break;
		
		case ROOT:
			strcat(buffer, "ROOT     ");
			strcat(buffer, "length: ");
			sprintf(l, "%d", node->data->list.length);
			strcat(buffer, l);
			break;
		
		case NODE:
			strcat(buffer, "NODE     ");
			strcat(buffer, "name: ");
			strcat(buffer, (const char*) node->data->node.name);
			strcat(buffer, ", value: ");
			if (node->data->node.value == NULL)
				strcat(buffer, "(null)");
			else
				strcat(buffer, (const char*) node->data->node.value);
			break;
		
		case NODELIST:
			strcat(buffer, "NODELIST ");
			strcat(buffer, "name: ");
			strcat(buffer, node->data->list.name);
			strcat(buffer, ", value: ");
			if (node->data->list.value == NULL)
				strcat(buffer, "(null)");
			else
				strcat(buffer, (const char*) node->data->list.value);
			strcat(buffer, ", length: ");
			sprintf(l, "%d", node->data->list.length);
			strcat(buffer, l);
			break;
	}
	
	return buffer;
}

// recoursive dump
int node_dump(node_t* root, int indent) {
	
	// check if this node has children.
	
	return 0;
}

int main(int argc, char* argv[]) {
	//printf("%d\n", nodeid);
	
	node_t* root = init_root(); 
	
	node_t* n = malloc(sizeof(node_t));
	n->nodeid = nodeid++;
	n->type = NODE;
	n->data = malloc(sizeof(node_u));
	n->data->node.name = "name";
	n->data->node.value = "val";
	
	// printf("%p %d: name: %s, value: %s\n", n, n->type, n->data->node.name, n->data->node.value);
	node_append(root, n);
	
	node_t* n1 = init_node("name1", NULL);
	//printf("%p %d: name: %s, value: %s\n", n1, n1->type, n1->data->node.name, n1->data->node.value);
	node_append(root, n1);
	
	node_t* nl1 = init_nodelist("nl1", "val1");
	//printf("%p %d: name: %s, value: %s\n", nl1, nl1->type, nl1->data->list.name, nl1->data->list.value);
	node_append(root, nl1);
	
	node_append(nl1, n1);
	
	/*
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
	
	//char nstr[100] = "";
	//node_to_str(nstr, n1);
	*/
	printf("==============================================\n\n");
	char buffer[101] = "";
	printf("%s\n", node_to_str(buffer, root));
	printf("%s\n", node_to_str(buffer, n));
	printf("%s\n", node_to_str(buffer, n1));
	printf("%s\n", node_to_str(buffer, nl1));
	
	node_dump(root, 0);
	
	return 0;
}
