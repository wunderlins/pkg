#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int a;
	int b;
	char** c;
} struct_t;

struct_t* init(int a, int b) {
	struct_t* s = malloc(sizeof(struct_t*));
	s->c = malloc(sizeof(char*) * 10);
	s->a = a;
	s->b = b;
	
	return s;
}

int main() {
	
	struct_t* s = init(1, 2);
	
	return 0;
}
