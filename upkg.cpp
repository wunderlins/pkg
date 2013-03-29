//============================================================================
// Name        : upkg.cpp
// Author      : Simon Wunderlin
// Version     : $Id: upkg.cpp 201 2012-01-10 19:57:58Z wus $
// Copyright   : GPL2
// Description : Generic package manager supporting: apt-get, macports, 
//               pacman (openbsd pkg, others?)
//============================================================================

/*
TODO: check for possible packagemanager on compile time.
TODO: find unix config file parser.
TODO: config file in ~/.config/upkg.conf or ~/.upkg.conf?
*/

#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "stringlib.h"
#include "upkg.h"

using namespace std;

string parser_pacman_last_name = "";
string parser_pacman_last_version = "";
string parser_port_last_name = "";
string parser_port_last_version = "";

/**
 * return name of env variable
 *
 * takes a line of the ENV and splits it by '=' (equal_p) and returns
 * the variable content (the reminder after '=').
 */
string env_name(int equal_p, string line) {
	return line.substr(0, equal_p);
}

/**
 * Split env line by '=' and return name
 */
string env_value(int equal_p, string line) {
	return line.substr(equal_p+1, line.length());
}

/**
 * Print usage information
 */
void usage(void) {
	cout << "Usage: " << cmd_opts.exec;
	cout << " <operation> [[[arg1] arg2] argN]" << endl;
	cout << "Version: " << STR(UPKG_VERSION) << ", " << STR(UPKG_MAINTAINER);
	cout << endl << endl;
	cout << "Unified Package Manager. This is a universal interface to *NIX ";
	cout << "command line" << endl << "package managers." << endl << endl;
	
	cout << "Use `sync` to keep your repository up to date, `list` to see what is";
	cout << " already" << endl << "installed. `search` to find available ";
	cout << "packages and `install` to install new"<< endl<<"packages.";
	cout << endl << endl;
	
	cout << "OPERATION" << endl;
	for(int i=0; i<KNOWN_OPERATIONS_LENGTH; i++) {
		cout << " " << STR(UPKG_OUTPUT) << known_operations_short[i] << "  " <<
		     known_operations[i] << " " << known_operations_info[i] << endl;
	}
	// cout << "  " << "version - display version" << endl << endl;
	cout << endl;

	cout << "ARGUMENTS" << endl;
	cout << " -h   display usage (this message)" << endl;
	cout << " -v   version of " << STR(UPKG_OUTPUT) << endl << endl;
	
	cout << "EXIT CODES" << endl;
//	cout << "  1   Argument Error." << endl;
	cout << "  2   No Package Manager found." << endl;
	cout << "  3   Unknown operation." << endl;
	cout << "  4   Failed to run Package Manager command" << endl;
	cout << "  5   Root privilages required" << endl << endl;
}

/** 
 * find a supported package manager
 */
int find_package_manager(void) {
	const string PATH = getenv("PATH");
	char * cstr, * path;
	cstr = new char[PATH.size()+1];
	strcpy(cstr, PATH.c_str());
	path = strtok(cstr, UPKG_PATH_DELIMITER);

	// loop over all paths in $PATH
	while (path != NULL) {
		// loop over possible commands
		for(int ii = 1; ii<KNOWN_COMMANDS_LENGTH; ii++) {
			string file = path;
			file += UPKG_PATH_SEPERATOR;
			file += known_commands[ii];
			if (DEBUGGING > 1)
				cout << "  " << file << endl;

			// check if file exists and is executable
			char * cstrfile; 
			cstrfile = new char[file.size()+1];
			strcpy(cstrfile, file.c_str());
			if(!access(cstrfile, X_OK)) {
				if (DEBUGGING > 1)
					cout << "Using: " << file << endl;
				// pkg_manager_file = file;
				globalargs.pkg_manager_file = file;
				return ii;
			}

			
		}
		path = strtok(NULL, UPKG_PATH_DELIMITER);
	}
	
	return UPKG_TYPE_UNKNOWN;
}

/**
 * apt output parser
 */
string parser_apt(int operation, string line) {
	
	if (operation == UPKG_MODE_SEARCH) { // search 
		// convert operation into to char for string concatenation
		// char operation_type[20]; sprintf(operation_type, "%d", operation);
		// char * operation_type; = int2char(operation);
		char operation_type[20]; int2char(operation, operation_type);
		int dash_pos = line.find_first_of(" ");
		// char dash_pos_str[20]; sprintf(dash_pos_str, "%d", dash_pos);
		//char * dash_pos_str = int2char(dash_pos);
		char dash_pos_str[20]; int2char(dash_pos, dash_pos_str);
		string package = line.substr(0, dash_pos);
		string description = line.substr(dash_pos+3, line.length()-1);
		string out = "";
		if (DEBUGGING > 1) {
			out += operation_type;
			out += " (";
			out += dash_pos_str;
			out += ") ";
			out += " -> ";
		}
		out += package;
		out += UPKG_OUTPUT_SEPERATOR;
		out += description;
		return out;
	}

	if (operation == UPKG_MODE_LIST) { // list
	
		// format: ii  name[ ]+version[ ]+description text
		// name and version do not include spaces, name, version and description 
		// fileds are delimited by a variable number of spaces.
		
		if (line.substr(2, 2) != "  ")
			return "";
		
		string mode = line.substr(0, 3);
		string reminder = line.substr(4, line.length());
		int delim_pos = reminder.find_first_of(" ");
		string name = reminder.substr(0, delim_pos);
		string v = ltrim(reminder.substr(delim_pos));
		int version_pos = v.find_first_of(" ");
		string version = v.substr(0, version_pos);
		string description = ltrim(v.substr(version_pos));
		return name + UPKG_OUTPUT_SEPERATOR + version + UPKG_OUTPUT_SEPERATOR + 
		       description;
	}
	
	return line;
}

/**
 * pacman output parser
 */
string parser_pacman(int operation, string line) {
	if (operation == UPKG_MODE_SEARCH) { // search 
		
		if (line.at(0) != ' ') {
			parser_pacman_last_name = line;
			int space_pos = line.find_first_of(" ");
			parser_pacman_last_name = line.substr(0, space_pos);
			return "";
		} else {
			return parser_pacman_last_name + UPKG_OUTPUT_SEPERATOR + ltrim(line); 
		}
		
		char operation_type[20]; int2char(operation, operation_type);
		int dash_pos = line.find_first_of(" ");
		char dash_pos_str[20]; int2char(dash_pos, dash_pos_str);
		return line;
	}

	if (operation == UPKG_MODE_LIST) { // list
	
		if (line == "")
			return "";
		
		if (line.substr(0, 4) == "Name") {
			int colon_pos = line.find_first_of(":");
			parser_pacman_last_name = line.substr(colon_pos+2);
			return "";
		}
		
		if (line.substr(0, 7) == "Version") {
			int colon_pos = line.find_first_of(":");
			parser_pacman_last_version = line.substr(colon_pos+2);
			return "";
		}
		
		if (line.substr(0, 11) == "Description") {
			int colon_pos = line.find_first_of(":");
			string description = line.substr(colon_pos+2);
			
			if (DEBUGGING > 1) {
				cout << "--> name: " << parser_pacman_last_name << endl;
				cout << "--> ver:  " << parser_pacman_last_version << endl;
				cout << "--> desc: " << description << endl;
			}
			
			// remove newlines at the end of strings
			parser_pacman_last_name = parser_pacman_last_name.substr(0,
			                          parser_pacman_last_name.size()-1);
			parser_pacman_last_version = parser_pacman_last_version.substr(0,
			                             parser_pacman_last_version.size()-1);
			//description = description.substr(0, description.size()-1);
			
			return parser_pacman_last_name + UPKG_OUTPUT_SEPERATOR + 
			       parser_pacman_last_version + UPKG_OUTPUT_SEPERATOR + 
				     description;
		}
		
		return "";
	}
	
	return line;
}

/**
 * macport output parser
 * FIXME: broken output of search
 */
string parser_macport(int operation, string line) {
	if (operation == UPKG_MODE_SEARCH) { // search 
		// cout << "-> " << line << endl;
		
		if (parser_port_last_name == "") {
			int space_pos = line.find_first_of("@");
			parser_port_last_name = line.substr(0, space_pos-2);
			return "";
		} else {
			string n = parser_port_last_name;
			parser_port_last_name = "";
			return n + UPKG_OUTPUT_SEPERATOR + line; 
		}
		
		return "";
	}

	if (operation == UPKG_MODE_LIST) { // list
		int delim_pos = line.find_first_of(" ");
		string name = line.substr(0, delim_pos);
		int at_pos = line.find_first_of("@");
		string reminder = line.substr(at_pos+1);
		int space_pos = reminder.find_first_of(" ");
		string version = reminder.substr(0, space_pos);
		string description = ltrim(reminder.substr(space_pos));
		return name + UPKG_OUTPUT_SEPERATOR + version + UPKG_OUTPUT_SEPERATOR + 
		       description;
	}
	
	return line;
}


/**
 * main function
 *
 * include env variables (envp), should work on POSIX and W32+
 */
int main(int argc, char **argv, char **envp) { 
	
	// parse command line
	cmd_opts = options_parse(argc, argv, CMD_OPTIONS);
	
	// display usage and exit
	if (options_isset(cmd_opts, 'h')) {
		usage();
		return EXIT_SUCCESS;
	}
	
	// display version and exit
	if (options_isset(cmd_opts, 'v')) {
		//char ver[] = STR(UPKG_VERSION);
		cout << STR(UPKG_VERSION) << endl;
		return EXIT_SUCCESS;
	}
	
	// output startup and environment information
	if (DEBUGGING) {
		// print known operations
		cout << "** KNOWN OPERATIONS:" << endl;
		cout << "(" << KNOWN_OPERATIONS_LENGTH;
		cout << ") ";
		for(int i=0; i<KNOWN_OPERATIONS_LENGTH; i++) {
			cout << known_operations[i];
			if (KNOWN_OPERATIONS_LENGTH-1 != i)
				cout << ", ";
		}
		cout << endl;
		
		// print argc
		cout << "** ARGUMENTS:" << endl;
		cout << "argc = " << cmd_opts.argc << endl;
		for(int i = 0; i < cmd_opts.argc; i++)
			cout << "argv[" << i << "] = " << cmd_opts.argv[i] << endl;
		
		// print env variables
		if (DEBUGGING > 1) {
			char** env;
			cout << "** ENVIRONMENT:" << endl;
			for (env = envp; *env != 0; env++) {
				string line = *env;
				int equal_p = first_index_of('=', line);
				string name = env_name(equal_p, line);
				string value = env_value(equal_p, line);
				
				cout << name << " = " << value << endl;
			}
		}

	}
	
	// check for supported package manager
	globalargs.pkg_manager = find_package_manager();
	
	if (globalargs.pkg_manager == UPKG_TYPE_UNKNOWN) {
		cout << "No package manager found. Check your $PATH variable." << endl;
		usage();
		return EXIT_NOPKM;
	}

	if (DEBUGGING > 1) {
		cout << "Package manager id: " << globalargs.pkg_manager << endl;
	}
	
	if (DEBUGGING) {
		cout << "** Package Manager: ";
		cout << globalargs.pkg_manager_file << endl;
	}
	
	// check cmd_opts.exec if it matches pki, pks, pku, etc.
	globalargs.operation = -1;
	
	if (cmd_opts.exec.length() == 3) {
		//cout << cmd_opts.exec << endl;
		
		char opt = cmd_opts.exec.at(2);
		//cout << opt << endl;
		
		for (int i = 0; i < KNOWN_OPERATIONS_LENGTH; i++) {
			if (known_operations_short[i].at(0) == opt) {
				globalargs.operation = i;
				break;
			}
		}
	}
		
	// check if we know the operation fomr arguments
	if (globalargs.operation == -1 && cmd_opts.argc > 1) {
		for(int i=0; i<KNOWN_OPERATIONS_LENGTH; i++) {
			if (DEBUGGING > 1)
				cout << "  +" << known_operations[i] << endl;
	
			if (known_operations[i] == cmd_opts.argv[1]) {
				globalargs.operation = i;
				// FIXME: shift argv[1], when invoking the exec as pk skip parameter 1
				break;
			}
		}

	}
	
	// if we haven't found a known operation in argv[1] stop the application
	if (globalargs.operation == -1) {
		cout << "Unknown operation: \"";
		if (argc > 1)
			cout << cmd_opts.argv[1];
		cout << "\"" << endl;
		usage();
		return EXIT_UNKNOWNOP;
	}
	
	// display usage and exit
	if (globalargs.operation == UPKG_MODE_HELP) {
		usage();
		return EXIT_SUCCESS;
	}

	// check if user is root for certain operations like install, sync, etc.
	if (globalargs.operation == UPKG_MODE_INSTALL ||
	    globalargs.operation == UPKG_MODE_SYNC || 
	    globalargs.operation == UPKG_MODE_UPGRADE ||
	    globalargs.operation == UPKG_MODE_REMOVE ||
	    globalargs.operation == UPKG_MODE_CLEANUP) {
		int user;
		user = getuid();
		if (user != 0) {
		  cout << "Error: must run as root with operation '" 
		       << known_operations[globalargs.operation] << "'" << endl;
			return EXIT_NOTUID0;
		}
	}
	if (DEBUGGING)
		cout << "Operation: " << globalargs.operation << ", " << cmd_opts.argv[1] << endl;
	
	// remeber remaining arguments in global string vector
	for(int i=2; i<argc; i++) {
		cmd_opts.arguments.push_back(argv[i]);
	}
	
	if (DEBUGGING > 1) {
		for(unsigned int i=0; i<cmd_opts.arguments.size(); i++) {
			cout << "  " << i << ": " << cmd_opts.arguments[i] << endl;
		}
	}
	
	// set the commandline 
	if (globalargs.operation == UPKG_MODE_SEARCH) {
		if (DEBUGGING > 1)
			cout <<  "* globalargs.operation == UPKG_MODE_SEARCH" << endl;
		globalargs.command = known_cmds_search[globalargs.pkg_manager];
	} else if (globalargs.operation == UPKG_MODE_LIST) {
		if (DEBUGGING > 1)
			cout <<  "* globalargs.operation == UPKG_MODE_LIST" << endl;
		globalargs.command = known_cmds_list[globalargs.pkg_manager];
	} else if (globalargs.operation == UPKG_MODE_INSTALL) {
		if (DEBUGGING > 1)
			cout <<  "* globalargs.operation == UPKG_MODE_INSTALL" << endl;
		globalargs.command = known_cmds_install[globalargs.pkg_manager];
	} else if (globalargs.operation == UPKG_MODE_SYNC) {
		if (DEBUGGING > 1)
			cout <<  "* globalargs.operation == UPKG_MODE_SYNC" << endl;
		globalargs.command = known_cmds_sync[globalargs.pkg_manager];
	} else if (globalargs.operation == UPKG_MODE_UPGRADE) {
		if (DEBUGGING > 1)
			cout <<  "* globalargs.operation == UPKG_MODE_UPGRADE" << endl;
		globalargs.command = known_cmds_upgrade[globalargs.pkg_manager];
	} else if (globalargs.operation == UPKG_MODE_REMOVE) {
		if (DEBUGGING > 1)
			cout <<  "* globalargs.operation == UPKG_MODE_REMOVE" << endl;
		globalargs.command = known_cmds_remove[globalargs.pkg_manager];
	} else if (globalargs.operation == UPKG_MODE_CLEANUP) {
		if (DEBUGGING > 1)
			cout <<  "* globalargs.operation == UPKG_MODE_CLEANUP" << endl;
		globalargs.command = known_cmds_remove[globalargs.pkg_manager];
	} 
	
	// append remaining options
	for(unsigned int i=0; i<cmd_opts.arguments.size(); i++) {
		globalargs.command += " ";
		globalargs.command += cmd_opts.arguments[i];
	}
	
	if (DEBUGGING)
		cout << "** Command: " << globalargs.command << endl;
	
	// run the command
	FILE *fp;
	char buffer[2048];

	// Open the command for reading. 
	fp = popen(string2char(globalargs.command), "r");
	if (fp == NULL) {
		cout << "Failed to run command\n";
		return EXIT_FAILEDTORUN;
	}
	
	if (globalargs.operation == UPKG_MODE_SEARCH || 
	    globalargs.operation == UPKG_MODE_LIST) {
		
		// Read the output char by char.
		while (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {
		
		 	// output parsing, needs implementation for every platform
		 	string l = "";
		 	l += buffer;
		 	
		 	if (globalargs.pkg_manager == UPKG_TYPE_APT || 
		 	    globalargs.pkg_manager == UPKG_TYPE_APTITUDE) {
			 	l = parser_apt(globalargs.operation, l);
			}
		
		 	if (globalargs.pkg_manager == UPKG_TYPE_PACMAN) {
			 	l = parser_pacman(globalargs.operation, l);
			}
		
		 	if (globalargs.pkg_manager == UPKG_TYPE_MACPORT) {
			 	l = parser_macport(globalargs.operation, l);
			}
			
			if (l == "")
				continue;
		
			cout << l << flush;
		}
		
	} else {
		// Read the output a line at a time - output it. 
		while ((signed int) fread(buffer, 1, 1, fp) != '\0') {
			cout << buffer << flush;
		}
		
	}
	
	pclose(fp);
	
	return EXIT_SUCCESS;
}


