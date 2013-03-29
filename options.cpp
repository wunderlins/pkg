//============================================================================
// Name        : options.cpp
// Author      : Simon Wunderlin
// Version     : $Id: options.cpp 183 2012-01-07 20:28:35Z wus $
// Copyright   : GPL2
// Description : command line options parsing utility
//============================================================================

#include "options.h"

using namespace std;

std::string options_value(cmd_options_t options, char opt) {
	
	for (int i = 0; i < options.optionc; i++)
		if (options.option[i] == opt) 
			return options.optarg[i];
	
	return "";
}

bool options_isset(cmd_options_t options, char opt) {

	for (int i = 0; i < options.optionc; i++)
		if (options.option[i] == opt)
			return true;
	
	return false;
}

cmd_options_t options_parse(int argc, char **argv, const char *getopts_opts) {
	
	cmd_options_t opts = {argc, argv, getopts_opts};
	opts.optionc = 0;
	int option_char;
	std::string tmp = argv[0];
	
	// resolve real absolut path on POSIX(?) systems
	char actualpath[PATH_MAX+1];
	char *ptr;
	ptr = realpath(opts.argv[0], actualpath);
	
	opts.exec = actualpath;
	
	// find last slash, and put only the exec name into the program var
	size_t len = opts.exec.length();
	size_t slash_pos = 0;
	
	// loop from end of string char by char until we find te first slash
	for (int i = len; i>=0; i--) {
		if (opts.exec.at(i-1) == '/') {
			slash_pos = i;
			break;
		}
	}
	
	if(slash_pos)
		opts.path = opts.exec.substr(0, slash_pos-1);
		//opts.exec = opts.exec.substr(slash_pos, len);
	
	slash_pos = 0;
	for (int i = strlen(argv[0]); i>=0; i--) {
		if (argv[0][i] == '/') {
			slash_pos = i;
			break;
		}
	}
	
	if (slash_pos)
		opts.exec = tmp.substr(slash_pos+1, tmp.length());
	else
		opts.exec = tmp;
	
	int i = 0;
	while ( (option_char = getopt(argc, argv, opts.getopts)) != EOF) {
		switch (option_char) {  
			case '?': fprintf(stderr, "Argument error");
				break;
			
			default:
				// cout << "option_char : " << option_char << endl;
				// cast int character representation to  char
				opts.option[i] = option_char; // '0'+option_char; 
				if (optarg != NULL)
					opts.optarg.push_back(optarg);
				else
					opts.optarg.push_back("");
				i++;
				opts.optionc++;
		}
	}

	for (int index = optind; index < argc; index++)
		opts.arguments.push_back(argv[index]);
		// printf ("Non-option argument %s\n", argv[index]);
	// printf("length: %d\n", l);

	return opts;
}
