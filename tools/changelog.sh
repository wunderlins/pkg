#!/usr/bin/env bash

tmpfile=/tmp/$$
target=doc-pak/CHANGELOG

./tools/svn2cl-0.13/svn2cl.sh \
                --output=$tmpfile \
                --authors=./tools/svn2cl-0.13/authors.xml \
                --group-by-day \
                --include-rev \
                $(svn info | awk '/^URL/ {print $2}') # target

echo -en "" > "$target"
echo "Change Log for $(svn info | awk '/^URL/ {print $2}')" >> "$target"
date=$(svn info | awk '/Last Changed Date/ {print $4" "$5" "$6}')
rev=$(svn info | awk '/Last Changed Rev/ {print $4}')
author=$(svn info | awk '/Last Changed Author/ {print $4}')
echo "Subversion Revision:" $rev $date $author >> "$target"
echo "Generation Date:" $(date "+%Y-%m-%d %H:%M:%S %z") >> "$target"
echo -en "________________________________________________________" >> "$target"
echo "________________________" >> "$target"
echo "" >> "$target"

cat "$tmpfile" >> "$target"
rm "$tmpfile"
