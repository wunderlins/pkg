//============================================================================
// Name        : class.h
// Author      : Simon Wunderlin
// Version     : $Id: class.h 62 2011-04-15 18:08:30Z wus $
// Copyright   : GPL2
// Description : Generic package manager supporting: apt-get, macports, 
//               pacman (openbsd pkg, others?)
//============================================================================

#ifndef CLASS_H
#define CLASS_H

#include <string>

using namespace std;

class parser_t {
	public:
	string line;
	string get_search(string);
	
};

#endif
