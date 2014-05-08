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
#include "pk.h"
#include "openbsd5.h"

/**
 * main function
 *
 * include env variables (envp), should work on POSIX and W32+
 */
int main(int argc, char **argv, char **envp) {
	
	// check action (index 2 in executable name)
	
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
