/**
 * Platform independent package manager
 *
 * Offers unified basic functionality for package management such as 
 * search/list for different unix operating systems.
 *
 * Goals:
 * - unified command set
 * - portable
 * - small
 * - eventually add unified output for responses
 *
 * (c) 2014 Simon Wunderlin, simon()wunderlinDTnet
 */

#include <stdio.h>
#include <string.h>
#include <libgen.h> // probably bsd only for `basename`
#include "pk.h"
#include "openbsd5.h"

/**
 * main function
 *
 * include env variables (envp), should work on POSIX and W32+
 */
int main(int argc, char **argv, char **envp) {
	
	// check action (index 2 in executable name)
	char *arg0 = basename(argv[0]);
	//printf("%d\n", (int) strlen(arg0));
	if (strlen(arg0) != 3) {
		printf("Wrong program name '%s' (%lu).\n", arg0, strlen(arg0));
		return 1;
	}
	
	char mode_c = '\0';
	mode_c = arg0[2];
	modes_t mode = NONE;
	/*
			pkm - update metadata
			pkc - list available updates
			pku - update all packages
			pkl - list all installed packages
			pks - search for package \$1
			pki - install package with name \$1
	*/
	switch(mode_c) {
		case 'm': mode = META;       break;
		case 'c': mode = UPDATEABLE; break;
		case 'u': mode = UPDATE;     break;
		case 'l': mode = LIST;       break;
		case 's': mode = SEARCH;     break;
		case 'i': mode = INSTALL;    break;
	}
	
	printf("%d\n", mode);
	
	// TODO: buffer remaining command line arguments and parameters
	
	// execute action as systemcall according to configuration
	FILE *fp;
	char *cmd = "read a; echo $a";
	char buffer[64];
	fp = popen(cmd, "r");
	if (fp == NULL) {
		printf("Failed\n");
	} else {
		size_t len = sizeof(buffer)-1;
		while (fgets(buffer, len, fp) != NULL) {
			printf("%s", buffer);
		}
		pclose(fp);
	}

	// TODO: return exitcode of the system call
	
	return 0;
}
