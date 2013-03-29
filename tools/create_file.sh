#!/usr/bin/env bash
# $Id: create_file.sh 170 2011-05-05 18:49:19Z wus $

if [ -z "$1" ]]; then
	echo "Usage: $0 <file>";
	exit 1;
fi

file="$1"

touch "$file"
chmod 700 "$file"
svn add "$file"
svn propset svn:keywords "Id Author" "$file"
gedit "$file"&
