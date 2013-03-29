#!/usr/bin/env bash
# $Id: svndiff_gui.sh 174 2011-05-05 19:13:55Z wus $

# use xxdiff when available and on X11

if [[ -n "$DISPLAY" && -x `which xxdiff` ]]; then
	svn stat
	read -p "Graphical diff? [Y/n]: " proceed
	if [[ "$proceed" == "" || "$proceed" == "y" || "$proceed" == "Y" ]]; then
		xx-svn-diff $@
		exit $?
	fi
fi

svn diff $@

