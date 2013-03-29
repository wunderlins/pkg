//============================================================================
// Name        : class.cpp
// Author      : Simon Wunderlin
// Version     : $Id: class.cpp 65 2011-04-16 17:44:22Z wus $
// Copyright   : GPL2
// Description : Class test
//============================================================================

#include <iostream>
#include <string>
#include <string.h>
#include <typeinfo>

#include "class.h"
#include "../stringlib.h"

using namespace std;

string parser_t::get_search(string line) {
	return line;
}

class parser_apt_t: public parser_t {
	public:
	string get_search(string);
};

string parser_apt_t::get_search(string line) {
	int space_pos = first_index_of(' ', line);
	char c[16]; int2char(space_pos, c);
	string out = "-> ";
	out += c;
	out += " | ";
	out += line;
		 
	return out;
}


int main(int argc, char **argv, char **envp) { 

	char stdin[1024] = "ibeet-dbg - E17 file chunk reading/writing library - debug symbols\n\
alsa-tools-gui - GUI based ALSA utilities for specific hardware\n\
e17 - The Enlightenment DR17 Window Manager\n\
e17-data - Enlightenment Window Manager Run Time Data Files\n\
e17-dbg - The Enlightenment DR17 Window Manager - debugging symbols\n\
e17-dev - Enlightenment headers, static libraries and documentation\n\
libefreet-dev - Development files for libefreet and libefreet-mime\n\
libefreet-doc - libefreet and libfreet-mime API documentation\n\
libefreet-svn-06 - Library that implements freedesktop.org specs for use with E17/EFL\n\
";
	
  parser_t* parser;
	if (false) {
		parser_t* p;
	  parser = p;
	  cout << "type: parser_t" << endl;
	} else {
		parser_apt_t* p;
	  parser = p;
	  cout << "type: parser_apt_t" << endl;
	}
  
	char * line;
	line = strtok(stdin, "\n");
	while (line != NULL) {
		cout << typeid(*parser).name() << " ";
		cout << (*parser).get_search(line) << endl;
		line = strtok(NULL, "\n");
	}
	
  return 0;
}

