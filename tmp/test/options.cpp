//============================================================================
// Name        : options.cpp
// Author      : Simon Wunderlin
// Version     : $Id: options.cpp 180 2012-01-04 22:09:38Z wus $
// Copyright   : GPL2
// Description : test for command line options parsing utility
//============================================================================

#include <string>
#include <iostream>
#include <stdio.h>
#include "../options.h"

using namespace std;

int main(int argc, char **argv, char **envp) {
	cmd_options_t parsed_options;
	parsed_options = options_parse(argc, argv, "hl:");
	
	cout << "=========================" << endl;
	cout << "argv[0]: " << parsed_options.argv[0] << endl;
	cout << "exec: " << parsed_options.exec << endl;
	cout << "path: " << parsed_options.path << endl;
	cout << "Number of options: " << parsed_options.optionc << endl;
	for (int i = 0; i < parsed_options.optionc; i++) {
		cout << "Option " << i << ": " << parsed_options.option[i] << " | ";
		cout << parsed_options.optarg[i] << endl << flush;
	}
	
	cout << "Number of arguments: " << parsed_options.arguments.size() << endl;
	for (unsigned int i = 0; i < parsed_options.arguments.size(); i++)
		cout << "Argument " << i << ": " << parsed_options.arguments[i] << endl;
	
	bool result = options_isset(parsed_options, 'h');
	cout << "Has option 'h': " << result << endl;
	result = options_isset(parsed_options, 'l');
	cout << "Has option 'l': " << result << " " 
	     << options_value(parsed_options, 'l') << endl;
	
	return EXIT_SUCCESS;
}

