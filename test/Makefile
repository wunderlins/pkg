.PHONY: strarray parray diff

strarray:
	gcc -DSTRARRAY_TESTCASE=1 -o strarray strarray.c

parray:
	gcc -DPARRAY_TESTCASE=1 -o parray parray.c

diff:
	git diff | colordiff | less -R

	