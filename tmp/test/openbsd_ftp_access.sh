#!/bin/bash

# default location:
# ftp://ftp.OpenBSD.org/pub/OpenBSD/
# ftp://mirror.switch.ch/pub/OpenBSD/
export PKG_PATH=ftp://mirror.switch.ch/pub/OpenBSD/`uname -r`/packages/`machine -a`/

echo $PKG_PATH;
