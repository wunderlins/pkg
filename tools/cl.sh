#!/usr/bin/env bash

d=`dirname $0`
$d/gitlog-to-changelog --format='[%h] %s' > doc/Changelog
echo -en "\n\n\n" >> doc/Changelog
cat doc/Changelog.svn >> doc/Changelog

