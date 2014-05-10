#ifndef _PK_OPENBSD5_H_
#define _PK_OPENBSD5_H_

const char *commands[PK_MODES_LENGTH] = {
	/* 0 */ "true",
	/* 1 */ "", // pkm - update metadata
	/* 2 */ "", // pkc - list available updates
	/* 3 */ "pkg_add -uUvi", // pku - update all packages
	/* 4 */ "pkg_info", // pkl - list all installed packages
	/* 5 */ "pkg_info -Q", // pks - search for package \$1
	/* 6 */ "pkg_add"  // pki - install package with name \$1
};
#endif

