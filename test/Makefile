.PHONY: strarray parray diff

CC      = gcc
CCFLAGS = -Wall -O3 
RM      = rm

strarray:
	$(CC) $(CCFLAGS) -DSTRARRAY_TESTCASE=1 -o strarray strarray.c

parray:
	$(CC) $(CCFLAGS) -DPARRAY_TESTCASE=1 -o parray parray.c

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

