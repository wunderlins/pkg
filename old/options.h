//============================================================================
// Name        : options.h
// Author      : Simon Wunderlin
// Version     : $Id: options.h 182 2012-01-05 20:42:05Z wus $
// Copyright   : GPL2
// Description : commandline options parsing
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <string>
#include <string.h>
#include <vector>

#ifndef CMD_OPTIONS
	#define CMD_OPTIONS ""
#endif

struct cmd_options_t {
	int argc;
	char ** argv;
	const char * getopts;

	char option[255];	
	int optionc;
	std::string exec; // the executable name with leading apths stripped from argv[0]
	std::string path; // full path to the executable
	std::vector<std::string> optarg;
	std::vector<std::string> arguments;
};

bool            options_isset(cmd_options_t, char);
std::string     options_value(cmd_options_t, char);
cmd_options_t   options_parse(int, char **, const char *);

