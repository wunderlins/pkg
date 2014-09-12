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

#include <inttypes.h> // required on linux for uintN_t

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef VERSION
	#define VERSION_STR TOSTRING(VERSION)
#else
	#define VERSION_STR "0"
#endif

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

int buffer_length = 254;
char output_line[2048] = "";
char mode_c = '\0';
modes_t mode = NONE;

// parameters expected
uint8_t expect_param[] = {
	/* 0 NONE */        0,
	/* 1 META */        0,
	/* 2 UPDATEABLE */  0,
	/* 3 UPDATE */      0,
	/* 4 LIST */        0,
	/* 5 SEARCH */      1,
	/* 6 INSTALL */     1
};

// requires root
uint8_t requires_root[] = {
	/* 0 NONE */        0,
	/* 1 META */        1,
	/* 2 UPDATEABLE */  0,
	/* 3 UPDATE */      1,
	/* 4 LIST */        0,
	/* 5 SEARCH */      0,
	/* 6 INSTALL */     1
};

/**
 * Function declarations
 */
int parse_output(char *);
int process_line(char *);
int append(char *, char, int);
int parse_output(char *);
void usage();

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

#ifdef Darwin
const char *commands[PK_MODES_LENGTH] = {
	/* 0 */ "echo 'Not implemented'",
	/* 1 */ "port selfupdate", // pkm - update metadata
	/* 2 */ "port outdated", // pkc - list available updates
	/* 3 */ "port upgrade outdated", // pku - update all packages
	/* 4 */ "port list", // pkl - list all installed packages
	/* 5 */ "port list", // pks - search for package \$1
	/* 6 */ "port install"  // pki - install package with name \$1
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
			printf("%s\n", line);
			break;

		case SEARCH:
			//printf("Mode: SEARCH\n");
			printf("%s\n", line);
			break;
	}
	
	//printf("%s [%d]\n", line, mode);
	fflush(stdout);
	return 0;
}
#endif

int append(char *str, char c, int index) {
	str[index] = c;
	str[index+1] = '\0';
	return 0;
}

int parse_output(char *buffer) {
	
	// printf("°[%d] %s", strlen(buffer), buffer);
	// return 1;
	
	// loop trough buffer and find newline characters
	int i = 0;
	for(i=0; i<buffer_length; i++) {
		if (buffer[i] == '\0') {
			output_line[0] = '\0';
			return 0;
		}
		
		if (buffer[i] == '\n') {
			printf("%s\n", output_line);
			output_line[0] = '\0';
			
			//printf("%c", buffer[i]);
		}
		
		//strcat(output_line, buffer[i]);
		//output_line[i] = buffer[i];
		// output_line[i+1] = '\0';
		append(output_line, buffer[i], i);
		continue;
		
		// terminate at the endo of a c string
		if (buffer[i] == '\0') {
			//strcat(output_line, &(buffer[i]));
			//printf("+[%d] %s %s\n", i, output_line, "End of String");
			//process_line(output_line);
			output_line[0] = '\0';
			break;
		}
		
		// process line if we have found a newline character
		if (buffer[i] == '\n') {
			//printf("-[%d] %s %s\n", i, output_line, "Newline");
			//process_line(output_line);
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
 * Usage information
 */
void usage() {
	//char *msg =
	
	printf("Platform independent package manager.\n\n"
				 "Usage: pks [name] # search package\n"
				 "       pki [name] # install package\n"
				 "       pkl        # list all installed packages\n"
				 "       pku        # update all packages\n"
				 "       pkc        # list available updates\n"
				 "       pkm        # update metadata\n"
				 "\n"
				 "ENVIRONMENT VARIABLES\n"
				 "  USE_SUDO: if set, sudo is automatically used for commands which usually \n"
				 "            need to be run by root.\n"
				 "\n"
				 "EXIT CODES\n"
				 "  1: Wrong executable name\n"
				 "  2: Failed to allocate memmory\n"
				 "  3: Missing parameter\n"
				 "  4: Unknown mode\n"
				 "\n"
				 "Version: %s\n"
				 "\n", VERSION_STR);
	return;
}


/**
 * main function
 *
 * include env variables (envp), should work on POSIX and W32+
 */
int main(int argc, char **argv, char **envp) {
	
	char* cuse_sudo = getenv("USE_SUDO");
	int use_sudo = 0;
	if (cuse_sudo != NULL) {
		//printf("env: %s\n", use_sudo);
		use_sudo = atoi(&cuse_sudo[0]);
		
		/*
		if (use_sudo == 1) {
			;
		}
		*/
	}
	//printf("Automatically using sudo: %d\n", use_sudo);
	
	// check action (index 2 in executable name)
	char *arg0 = basename(argv[0]);
	
	//printf("%d\n", (int) strlen(arg0));
	if (strlen(arg0) != 3) {
		printf("Wrong program name '%s' (%lu).\n\n", arg0, strlen(arg0));
		usage();
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
		default: usage(); return 4;         // catch unknown modes
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
	
	// check parameter count
	if (expect_param[mode] != i-1) {
		//printf("num of parameters: %d, expected: %d, mode: %d\n", i-1, expect_param[mode], mode);
		usage();
		return 3;
	}
	
	// check if we need super user rights
	// TODO: implement
	/*
	if (requires_root[mode]) {
		
		return 4;
	}
	*/
	
	// run the command
	int rc = -1;
	FILE *fp;
	char buffer[buffer_length];
	char cmd[strlen(commands[mode]) + strlen(params) + 1 + 5];
	cmd[0] = '\0';
	
	// use sudo if required
	//printf("requires_root: %d, use_sudo: %d\n", requires_root[mode], use_sudo);
	if (requires_root[mode] && use_sudo == 1) {
		//printf("sudo ...\n");
		const char* sudo = "sudo ";
    strcat(cmd, sudo);
	}
	
	strcat(cmd, commands[mode]);
	strcat(cmd, params);
	printf("cmd: %s\n", cmd);
	
	fflush(stdout); // OpenBSD pclose manual under BUGS
	fp = popen(cmd, "r");
	if (fp == NULL) {
		printf("Failed\n");
	} else {
		size_t len = sizeof(buffer)-1;
		while (fgets(buffer, len, fp) != NULL) {
			
			// TODO: parse output
			//printf("%s", buffer); return 1;
			parse_output(buffer);
		}
		rc = pclose(fp);
		//printf("%d\n", rc);
	}

	// return exitcode of the system call
	if (rc > 254) rc = 254;
	return rc;
}
