#!/bin/bash
# $Id: svndiff.sh 110 2011-04-21 16:02:58Z wus $

svn diff|colordiff|less -R
