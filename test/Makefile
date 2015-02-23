.PHONY: strarray parray diff datastruct nested_struct

CC      = gcc
CCFLAGS = -Wall -O3 
RM      = rm

strarray:
	$(CC) $(CCFLAGS) -DSTRARRAY_TESTCASE=1 -o strarray strarray.c

datastruct:
	$(CC) $(CCFLAGS) -std=c99 -o datastruct datastruct.c

parray:
	$(CC) $(CCFLAGS) -DPARRAY_TESTCASE=1 -o parray parray.c

nested_struct:
	$(CC) $(CCFLAGS) -o nested_struct nested_struct.c

diff:
	git diff | colordiff | less -R


clean:
	$(RM) class || true
	$(RM) options || true
	$(RM) popen || true
	$(RM) strarray || true
	$(RM) parray || true
	$(RM) vector || true
	$(RM) struct || true
	$(RM) *~

