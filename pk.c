/**
 * Platform independent package manager
 *
 * Offers unified basic functionality for package management such as 
 * search/list for different unix operating systems.
 *
 * Goals:
 * - unified command set, DONE
 * - portable, DONE
 * - small, DONE
 * - eventually add unified output for responses
 *
 * (c) 2014 Simon Wunderlin, simon()wunderlinDTnet
 */

#include <stdio.h>
#include <string.h>
#include <libgen.h> // probably bsd only for `basename`
#include <stdlib.h> // used for realloc

#include <inttypes.h> // required on linux for uintN_t
// #include "strarray.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef VERSION
	#define VERSION_STR TOSTRING(VERSION)
#else
	#define VERSION_STR "0"
#endif

#define PK_MODES_LENGTH 7
#define OUTPUT_DELIMITER "|"

#define NUM_TOKENS 5
#define MIN_SPACES 2
typedef char tokens[NUM_TOKENS][80];

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
int tokenize(tokens, char*);
int parse_output(char *);
void process_line(char *);
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

void process_line(char *line) {
	int c = 0;
	int s = 0;
	char name[100] = "";
	char desc[100] = "";
	size_t len = strlen(line);
	
	switch(mode) {
		case NONE:
		case META:
		case UPDATEABLE:
		case UPDATE:
		case INSTALL:
		default:
			printf("%s\n", line);
			break;
			
		case SEARCH:
			printf("%s%s\n", line, OUTPUT_DELIMITER);
			break;
		
		case LIST:
			// find first space
			while(line[c++] != ' '); // first space
			s = c;
			while(line[s++] == ' '); // beginning of next string
			
			memcpy(name, &line[0], c-1);
			memcpy(desc, &line[s-1], len-s+1);
			
			printf("%s%s%s\n", name, OUTPUT_DELIMITER, desc);
			break;
	}
	
	fflush(stdout);
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

void process_line(char *line) {
	
	tokens result;
	int count;
	
	switch(mode) {
		case NONE:
		case META:
		case UPDATEABLE:
		case UPDATE:
		case INSTALL:
		default:
			printf("%s\n", line);
			break;
			
		case LIST:
		case SEARCH:
			count = tokenize(result, line);
			printf("%s%s%s\n", result[0], OUTPUT_DELIMITER, result[2]);
			break;
	}
	
	fflush(stdout);
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

void process_line(char *line) {
	
	//printf("%s\n", line);
	
	tokens result;
	int i;
	size_t len = strlen(line);
	char name[80] = "";
	char desc[120] = "";
	
	switch(mode) {
		case NONE:
		case META:
		case UPDATEABLE:
		case UPDATE:
		case INSTALL:
		default:
			printf("%s\n", line);
			break;

		case LIST:
			/**
			 * example output line:
			 * ii  accountsservice                       0.6.21-8                           amd64        query and manipulate user account information
			 */
			
			// char 2,3 must be ' '
			if (len > 3 && !(line[2] == ' ' && line[3] == ' '))
				break;
			
			tokenize(result, line);
			printf("%s%s%s\n", result[1], OUTPUT_DELIMITER, result[4]);
			
			break;
		
		case SEARCH:
			// find the first occourence of " - ", split there.
			// first token is the package name, 2nd token is the package's 
			// description 
			
			for(i=0; i<len; i++) {
				// if (line[i] == '-') printf("%d\n", i);
					
				if (line[i] == ' ' && line[i+1] == '-' && line[i+2] == ' ') {
					// found delimiter
					memcpy(desc, &line[i+3], len-i-2);
					break;
				}
				
				name[i] = line[i];
				name[i+1] = '\0';
			}
			
			printf("%s%s%s\n", name, OUTPUT_DELIMITER, desc);
			break;
	}
	
	fflush(stdout);
}
#endif

/**
 * Split string by susequent spaces
 *
 * returns the number of elements
 */
int tokenize(tokens result, char* string) {
	
	char last;
	int space_count = 0;
	int i = 0;
	int word = 0;
	int length = strlen(string);
	int buffer_l = 0;
	char buffer[100] = "";
	
	// loop over the array character by character
	for (i=0; i<length; i++) {
		last = string[i];
		
		//printf("--->%c %d\n", last, space_count);
		
		if (last == ' ')
			space_count++;
		else
			space_count = 0;
		
		if (space_count >= MIN_SPACES) {
			buffer[buffer_l-1] = '\0'; // remove last space from buffer
			strcpy(result[word++], buffer);
			//printf("--> %s\n", buffer);
			
			// clear buffer
			buffer[0] = '\0';
			buffer_l = 0;
			
			// is the result array full?
			if (word == NUM_TOKENS) {
				break;
			}
			
			// find next non space character
			for (; string[i] == ' '; i++);
			space_count = 0;
			i--;
			continue;
		}
		
		// remeber character
		buffer[buffer_l++] = last;
		buffer[buffer_l] = '\0';
	}
	
	// copy last buffer into the result
	if (buffer[0] != '\0') {
		strcpy(result[word++], buffer);
		word++;
	}
	
	return word-1;
}

/**
 * Append a character to char array
 */
int append(char *str, char c, int index) {
	str[index] = c;
	str[index+1] = '\0';
	return 0;
}

/**
 * parse outut stream from popen
 *
 * split stream by '\n', run line parser on any line. the line parser is
 * package manager dependent
 */
int parse_output(char *buffer) {
	
	// loop through buffer and find newline characters
	int i = 0;
	for(i=0; i<buffer_length; i++) {
		if (buffer[i] == '\0') {
			output_line[0] = '\0';
			return 0;
		}
		
		if (buffer[i] == '\n') {
			//printf("%s\n", output_line);
			process_line(output_line);
			output_line[0] = '\0';
			
			//printf("%c", buffer[i]);
		}
		
		append(output_line, buffer[i], i);
		continue;
	}
	
	return 0;
}

/**
 * Usage information
 */
void usage() {
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
	}
	
	// check action (index 2 in executable name)
	char *arg0 = basename(argv[0]);
	
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

	// abort if there isn't a command defined
	if (strcmp(commands[mode], "") == 0) {
		printf("Command not available on this platform\n");
		exit(0);
	}

	
	// buffer remaining command line arguments and parameters
	char *params = "";
	int i, len = 0;
	for (i=1; i<argc; i++) {
		len = len + sizeof(argv[i]) + sizeof(" ");
		char *tmp;
		if ((tmp = malloc(sizeof(params) + len + 1)) == NULL) {
			printf("Failed to allocate memory.\n");
			return 2;
		}
		strcpy(tmp, params);
		strcat(tmp, " ");
		strcat(tmp, argv[i]);
		
		// add asterisk for port at the end of the search parameter
		#ifdef Darwin
		if (mode == SEARCH)
			strcat(tmp, "*");
		#endif
		params = tmp;
		
		//printf("params: %s\n", params);
	}
	
	// check parameter count
	if (expect_param[mode] != i-1) {
		//printf("num of parameters: %d, expected: %d, mode: %d\n", i-1, expect_param[mode], mode);
		usage();
		return 3;
	}
	
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
			
			// parse output
			parse_output(buffer);
		}
		rc = pclose(fp);
		//printf("%d\n", rc);
	}

	// return exitcode of the system call
	if (rc > 254) rc = 254;
	return rc;
}
