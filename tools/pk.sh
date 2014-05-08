#!/usr/bin/env bash
# PACKAGE MANAGEMENT SYSTEM
# 
# Offers basic command to manage system updates and packages for:
#  - debian linux 7+
#  - openbsd 5.5+
#  - (osx with ports)
#  
# DESIRED COMMANDS 
#  
#  m) update meta data      packages
#  c) updates available?    os/packages
#  u) update                os/packages
#  l) list                  packages
#  s) search                packages
#  i) install               packages
# 
# COMMANDS DEBIAN
# 
#  m) sudo apt-get update
#  c) apt-get -Vs upgrade | perl -ne 'print if /upgraded:/ .. /upgraded,/'
#  u) sudo apt-get -y upgrade
#  l) dpkg -l
#  s) apt-cache search "$1"
#  i) sudo apt-get -y install "$1"
#
# create links:
# for c in $(echo "m c u l s i" | tr " " "\n"); do ln -s pk pk$c; done
#
# (c) 2014, Simon Wunderlin, swunderlin()gmail[DT]com
#

DEBUGGING=0 # 0 turns debug output off

function usage {
	cat <<- EOF
	
		usage: pk [param]
		
			pkm - update metadata
			pkc - list available updates
			pku - update all packages
			pkl - list all installed packages
			pks - search for package \$1
			pki - install package with name \$1

EOF
}

if [[ $DEBUGGING > 0 ]]; then
	echo "Debug level:    $DEBUGGING"
	echo "Bash Version:   $BASH_VERSION"
fi

## detect system
G_OS=`uname`
G_OS_LONG=`uname -a`
G_SYSTEM="Unknown"
if [[ `uname` == "OpenBSD" ]]; then
	G_SYSTEM="OpenBSD"
elif [[ `echo "$G_OS_LONG" | grep "Debian"` ]]; then
	G_SYSTEM="Debian"
fi

if [[ $DEBUGGING > 0 ]]; then
	echo "System:         $G_SYSTEM";
fi

## setup environment
# default commands
C_M=""
C_C=""
C_U=""
C_L=""
C_S=""
C_I=""

if [[ "$G_SYSTEM" == "OpenBSD" ]]; then
	
	function openbsd_update {
		for p in `pkg_info | sed -e 's/-[0-9\.]*.*$//'`; do 
			sudo pkg_add -v -s -u $p | grep -v '(ok)' 
		done
	}
	
	function openbsd_search {
		pkg_info -Q $1 | sed -e 's|-\([0-9]\)|	\1|'
	}
	
	function openbsd_list {
		pkg_info | sed -e 's|-\([0-9]\)|	\1|'
	}
	
	function openbsd_update {
		sudo pkg_add -uUvi | grep -v '(ok)'
	}
	
	C_M=''
	C_C='openbsd_update'
	C_U='openbsd_update'
	C_L='openbsd_list'
	C_S='openbsd_search'   # param $1 is search term
	C_I='sudo pkg_add'     # param $1 is package name
elif [[ "$G_SYSTEM" == "Debian" ]]; then
	C_M='sudo apt-get update'
	C_C='apt-get -Vs upgrade | perl -ne ''print if /upgraded:/ .. /upgraded,/'''
	C_U='sudo apt-get -y upgrade'
	C_L='dpkg -l'
	C_S='apt-cache search'          # param $1 is search term
	C_I='sudo apt-get -y install'   # param $1 is package name
fi

## check action
if [[ $DEBUGGING > 0 ]]; then
	echo "Meta:      $C_M"
	echo "Check:     $C_C"
	echo "Update:    $C_U"
	echo "List:      $C_L"
	echo "Search:    $C_S"
	echo "Install:   $C_I"
fi

## run action
EXE=`basename $0`
if [[ "$EXE" == "pkm" ]]; then
	$C_M;
elif [[ "$EXE" == "pkc" ]]; then
	$C_C;
elif [[ "$EXE" == "pku" ]]; then
	$C_U;
elif [[ "$EXE" == "pkl" ]]; then
	$C_L;
elif [[ "$EXE" == "pks" ]]; then
	$C_S $1;
elif [[ "$EXE" == "pki" ]]; then
	$C_I $1;
else
	usage;
fi


