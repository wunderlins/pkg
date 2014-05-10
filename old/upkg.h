//============================================================================
// Name        : upkg.h
// Author      : Simon Wunderlin
// Version     : $Id: upkg.h 194 2012-01-09 21:14:16Z wus $
// Copyright   : GPL2
// Description : Generic package manager header
//============================================================================

#ifndef UPKG_H
#define UPKG_H

// TODO: finish rpm/yum implementation
// TODO: test all operations on all target systems

#define UPKG_TYPE_UNKNOWN 0
#define UPKG_TYPE_APT 1
#define UPKG_TYPE_APTITUDE 2
#define UPKG_TYPE_PACMAN 3
#define UPKG_TYPE_MACPORT 4
#define UPKG_TYPE_RPM 5

#define UPKG_MODE_SEARCH 0
#define UPKG_MODE_LIST 1
#define UPKG_MODE_INSTALL 2
#define UPKG_MODE_HELP 3
#define UPKG_MODE_SYNC 4
#define UPKG_MODE_UPGRADE 5
#define UPKG_MODE_REMOVE 6
#define UPKG_MODE_CLEANUP 7

#define UPKG_PATH_DELIMITER ":"
#define UPKG_PATH_SEPERATOR "/"

#define UPKG_OUTPUT_SEPERATOR "	"

#define CMD_OPTIONS "hv"

#ifndef UPKG_DEBUG
#define UPKG_DEBUG 0
#endif

#define EXIT_NOPKM 2
#define EXIT_UNKNOWNOP 3
#define EXIT_FAILEDTORUN 4
#define EXIT_NOTUID0 5

// convert define into string
#define STR1(x) #x
#define STR(x) STR1(x)

#include "options.h"

// 1 DEBUG, 2 VERBOUSE
int DEBUGGING = UPKG_DEBUG; 

// command line parser result
cmd_options_t cmd_opts;

// known programms and operations
#define  KNOWN_OPERATIONS_LENGTH 8
const std::string known_operations[KNOWN_OPERATIONS_LENGTH] = {
	"search", 
	"list", 
	"install", 
	"help", 
	"update", 
	"upgrade",
	"remove",
	"cleanup",
};

const std::string known_operations_short[KNOWN_OPERATIONS_LENGTH] = {
	"s", 
	"l", 
	"i", 
	"h", 
	"u", 
	"p",
	"r",
	"c",
};

// operation parameters
const std::string known_operations_info[KNOWN_OPERATIONS_LENGTH] = {
	"<search string> - search for one or more packages", 
	"- list installed packages", 
	"<package1> [package2] [...] - install one or many packages", 
	"- this help",
	"- synchronize metadata, get latest package informations",
	"- upgrade system",
	"<package1> [package2] [...] - Remove package",
	"- cleanup"
};

#define KNOWN_COMMANDS_LENGTH 6
const std::string known_commands[KNOWN_COMMANDS_LENGTH] = {
	"Unknown", 
	"apt-get",  // debian apt-get
	"aptitude", // debian aptitude
	"pacman",   // arch pacman
	"port",     // MacPorts port
	"rpm",      // redhat's rpm
};

const std::string known_cmds_search[KNOWN_COMMANDS_LENGTH] = {
	"", 
	"apt-cache search", // debian apt-get
	"aptitude search",  // debian aptitude
	"pacman -Ss",       // arch pacman
	"port search",      // MacPorts port
	"yum search"        // redhat rpm, TEST: rpm search
};

const std::string known_cmds_list[KNOWN_COMMANDS_LENGTH] = {
	"", 
	"dpkg -l",             // debian apt-get
	"dpkg -l",             // debian aptitude
	"pacman -Qi",          // arch pacman
	"port list installed", // MacPorts port
	"rpm -q"               // redhat rpm, TEST: rpm list
};

const std::string known_cmds_install[KNOWN_COMMANDS_LENGTH] = {
	"", 
	"apt-get install",   // debian apt-get
	"aptitude install",  // debian aptitude
	"pacman -S",         // arch pacman
	"port install",      // MacPorts port
	""                   // redhat rpm, TODO: implement rpm install
};

const std::string known_cmds_sync[KNOWN_COMMANDS_LENGTH] = {
	"", 
	"apt-get update",   // debian apt-get
	"aptitude update",  // debian aptitude
	"pacman -Sy",       // arch pacman
	"port selfupdate",  // MacPorts port
	""                   // redhat rpm, TODO: implement rpm sync
};

const std::string known_cmds_upgrade[KNOWN_COMMANDS_LENGTH] = {
	"", 
	"apt-get upgrade",   // debian apt-get TEST: apt-get upgrade package
	"aptitude upgrade",  // debian aptitude TEST: aptitude upgrade package
	"pacman -Syu",       // arch pacman TEST: pacman upgrade package
	"port upgrade all",    // MacPorts port TEST: port upgrade package
	""                   // redhat rpm, TODO: implement rpm remove
};

const std::string known_cmds_remove[KNOWN_COMMANDS_LENGTH] = {
	"", 
	"apt-get remove",    // debian apt-get
	"aptitude remove",   // debian aptitude
	"pacman -Rcs",       // arch pacman
	"port uninstall",  // MacPorts port, FIXME: macport upgrade fails: 2Pong is not installed
	""                   // redhat rpm, TODO: implement rpm upgrade 
};

const std::string known_cmds_cleanup[KNOWN_COMMANDS_LENGTH] = {
	"", 
	"apt-get autoremove",    // debian apt-get
	"aptitude autoremove",   // debian aptitude
	"",       // arch pacman, , TODO: implement arch cleanup
	"",  // MacPorts port, TODO: implement macports cleanup
	""                   // redhat rpm, TODO: implement rpm cleanup
};

// global parameters
struct globalargs_t {
    int operation;                       // what to do?
//    char **argv;                         // main argv
//    char **envp;                         // main argc
//    std::vector<std::string> arguments;  // additional arguments
    int pkg_manager;                     // which package manager found?
    std::string pkg_manager_file;        // path to package manager binary
    std::string command;                 // shell commando to execute
} globalargs;

// function declarations

/**
 * Print usage information
 */
void usage(void);

/**
 * return name of env variable
 *
 * takes a line of the ENV and splits it by '=' (equal_p) and returns
 * the variable content (the reminder after '=').
 */
std::string env_name(int, std::string);

/**
 * Split env line by '=' and return name
 */
std::string env_value(int, std::string);

/** 
 * find a supported package manager
 */
int find_package_manager(void);

/**
 * apt output parser
 */
std::string parser_apt(int, std::string);

/**
 * pacman output parser
 */
std::string parser_pacman(int, std::string);

/**
 * macport output parser
 */
std::string parser_macport(int, std::string);

#endif

