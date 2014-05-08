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
	char mode = '\0';
	char *arg0 = basename(argv[0]);
	if (strlen(arg0) != 3) {
		printf("Wrong program name '%s' (%lu).\n", arg0, strlen(arg0));
		return 1;
	}
	mode = arg0[2];
	
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

	// return exitcode of the system call
	
	return 0;
}
