#ifndef _PK_DEBIAN_H_
#define _PK_DEBIAN_H_

const char *commands[PK_MODES_LENGTH] = {
	/* 0 */ "true",
	/* 1 */ "apt-get update", // pkm - update metadata
	/* 2 */ "apt-get -Vs upgrade | perl -ne 'print if /upgraded:/ .. /upgraded,/'", // pkc - list available updates
	/* 3 */ "apt-get upgrade", // pku - update all packages
	/* 4 */ "dpkg -l", // pkl - list all installed packages
	/* 5 */ "apt-cache search", // pks - search for package \$1
	/* 6 */ "apt-get install"  // pki - install package with name \$1
};
#endif

