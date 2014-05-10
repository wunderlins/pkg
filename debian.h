#ifndef _PK_DEBIAN_H_
#define _PK_DEBIAN_H_

const char *commands[PK_MODES_LENGTH] = {
	/* 0 */ "", // pkm - update metadata
	/* 1 */ "", // pkc - list available updates
	/* 2 */ "pkg_add -uUvi", // pku - update all packages
	/* 3 */ "pkg_info", // pkl - list all installed packages
	/* 4 */ "pkg_info -Q", // pks - search for package \$1
	/* 5 */ "pkg_add"  // pki - install package with name \$1
};
#endif

