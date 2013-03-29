#!/bin/bash
# $Id: todo.sh 183 2012-01-07 20:28:35Z wus $

FNAME=""
IFS="
"
target="./doc-pak/TODO"
exclude='.svn|todo.sh|svn2cl|CHANGELOG|TODO'

echo -en "" > "$target"
echo "Change Log for $(svn info | awk '/^URL/ {print $2}')" >> "$target"
date=$(svn info | awk '/Last Changed Date/ {print $4" "$5" "$6}')
rev=$(svn info | awk '/Last Changed Rev/ {print $4}')
author=$(svn info | awk '/Last Changed Author/ {print $4}')
echo "Subversion Revision:" $rev $date $author >> "$target"
echo "Generation Date:" $(date "+%Y-%m-%d %H:%M:%S %z") >> "$target"
echo -en "________________________________________________________" >> "$target"
echo "________________________" >> "$target"

display_log_message() {
	line=$(echo "$1" | sed -e 's/^[^:]\+//')
	num=$(echo "$line" | sed -e 's/^:\([^:]\+\).*/\1/')
	comment=$(echo "$line" | sed -e 's/^[^\:]*\://' -)
	comment=$(echo "$comment" | sed -e 's/.*FIXME:/FIXME/')
	comment=$(echo "$comment" | sed -e 's/.*TODO:/TODO /')
	comment=$(echo "$comment" | sed -e 's/.*TEST:/TEST /')
	printf "%5s " $num >> "$target"
	echo "$comment" >> "$target"
}


for l in $(egrep -n -R "FIXME:|TODO:|TEST:" .); do
	f=$(echo "$l" | cut -d':' -f1)
	if [[ -L "$f" ]]; then continue; fi
	if [[ ! -f "$f" ]]; then continue; fi
	if [[ -x "$f" ]]; then continue; fi
	echo "$f" | egrep "$exclude" >/dev/null && continue;
	
	if [[ "$f" != "$FNAME" ]]; then 
		FNAME=$f;
		echo >> "$target"
		echo "$FNAME" | sed -e 's/^\.\///' - >> "$target"
	fi
	display_log_message "$l"
	
done
