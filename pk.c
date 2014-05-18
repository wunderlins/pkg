/**
 * Platform independent package manager
 *
 * Offers unified basic functionality for package management such as 
 * search/list for different unix operating systems.
 *
 * Goals:
 * - unified command set, DONE
 * - portable
 * - small
 * - eventually add unified output for responses
 *
 * (c) 2014 Simon Wunderlin, simon()wunderlinDTnet
 */

#include <stdio.h>
#include <string.h>
#include <libgen.h> // probably bsd only for `basename`
#include <stdlib.h> // used for realloc

#define PK_MODES_LENGTH 7

typedef enum {
	/* 0 */ NONE, 
	/* 1 */ META, 
	/* 2 */ UPDATEABLE, 
	/* 3 */ UPDATE, 
	/* 4 */ LIST, 
	/* 5 */ SEARCH, 
	/* 6 */ INSTALL
} modes_t;

int buffer_length = 3;
char output_line[2048] = "";
char mode_c = '\0';
modes_t mode = NONE;

/**
 * Function declarations
 */
int parse_output(char *);
int process_line(char *);

#ifdef OpenBSD
const char *commands[PK_MODES_LENGTH] = {
	/* 0 */ "echo 'Not implemented'",
	/* 1 */ "", // pkm - update metadata
	/* 2 */ "", // pkc - list available updates
	/* 3 */ "pkg_add -uUvi", // pku - update all packages
	/* 4 */ "pkg_info", // pkl - list all installed packages
	/* 5 */ "pkg_info -Q", // pks - search for package \$1
	/* 6 */ "pkg_add"  // pki - install package with name \$1
};

int process_line(char *line) {
	printf("%s\n", line);
	fflush(stdout);
	return 0;
}
#endif

#ifdef Debian
const char *commands[PK_MODES_LENGTH] = {
	/* 0 */ "echo 'Not implemented'",
	/* 1 */ "apt-get update", // pkm - update metadata
	/* 2 */ "apt-get -Vs upgrade | perl -ne 'print if /upgraded:/ .. /upgraded,/'", 
	        // pkc - list available updates
	/* 3 */ "apt-get upgrade", // pku - update all packages
	/* 4 */ "dpkg -l", // pkl - list all installed packages
	/* 5 */ "apt-cache search", // pks - search for package \$1
	/* 6 */ "apt-get install"  // pki - install package with name \$1
};

int process_line(char *line) {
		
	switch(mode) {
		case NONE:
		case META:
		case UPDATEABLE:
		case UPDATE:
		case LIST:
		case INSTALL:
		default:
			printf("%s\n", line, mode);
			break;

		case SEARCH:
			//printf("Mode: SEARCH\n");
			printf("%s\n", line, mode);
			break;
	}
	
	//printf("%s [%d]\n", line, mode);
	fflush(stdout);
	return 0;
}
#endif

int parse_output(char *buffer) {
	
	// loop trough buffer and find newline characters
	int i = 0;
	for(i=0; i<buffer_length; i++) {
		// terminate at the endo of a c string
		if (buffer[i] == '\0') {
			// printf("[%d] %s\n", i, "End of String");
			break;
		}
		
		// process line if we have found a newline character
		if (buffer[i] == '\n') {
			//printf("\n[%d] %s", i, "Newline");
			process_line(output_line);
			output_line[0] = '\0';
			break;
		}
		
		strcat(output_line, &(buffer[i]));
	}
	
	//printf("%s", buffer);
	//fflush(stdout);
	
	return 0;
}

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
	
	mode_c = arg0[2];
	
	switch(mode_c) {
		case 'm': mode = META;       break; // pkm - update metadata
		case 'c': mode = UPDATEABLE; break; // pkc - list available updates
		case 'u': mode = UPDATE;     break; // pku - update all packages
		case 'l': mode = LIST;       break; // pkl - list all installed packages
		case 's': mode = SEARCH;     break; // pks - search for package \$1
		case 'i': mode = INSTALL;    break; // pki - install package with name \$1
	}
	
	// buffer remaining command line arguments and parameters
	char *params = "";
	int i, len = 0;
	for (i=1; i<argc; i++) {
		len = len + sizeof(argv[i]) + sizeof(" ");
		char *tmp;
		if ((tmp = malloc(sizeof(params) + len + 1)) == NULL) {
			printf("Failed to allocate memmory.\n");
			return 2;
		}
		strcpy(tmp, params);
		strcat(tmp, " ");
		strcat(tmp, argv[i]);
		
		params = tmp;
	}
	
	// run the command
	int rc = -1;
	FILE *fp;
	char buffer[buffer_length];
	char cmd[strlen(commands[mode]) + strlen(params) + 1];
	strcpy(cmd, commands[mode]);
	strcat(cmd, params);
	//printf("cmd: %s\n", cmd);
	
	fflush(stdout); // OpenBSD pclose manual under BUGS
	fp = popen(cmd, "r");
	if (fp == NULL) {
		printf("Failed\n");
	} else {
		size_t len = sizeof(buffer)-1;
		while (fgets(buffer, len, fp) != NULL) {
			
			// TODO: parse output
			parse_output(buffer);
		}
		rc = pclose(fp);
		//printf("%d\n", rc);
	}

	// return exitcode of the system call
	if (rc > 255) rc = 255;
	return rc;
}
