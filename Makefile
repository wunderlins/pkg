# GOAL: compile only platform specific functionality to keep it small

.PHONY: changelog

VERSION = "0.2"
CC      = gcc
CCFLAGS = -Wall -O3 
RM      = rm
#	os      = $(shell uname -o 2>/dev/null || echo 0)

#CCFLAGS += -D OpenBSD
#CCFLAGS += -D Debian

ifeq ($(shell uname),Darwin)
		OS = Darwin
else
	ifeq ($(shell uname -o),GNU/Linux)
		OS = Debian
	else
		OS = OpenBSD
	endif
endif

CCFLAGS += -D $(OS) -D VERSION=$(VERSION)

all:
	@echo "Target OS: " $(OS)
	$(CC) $(CCFLAGS) -o pk pk.c
	#strip pk
	[ ! -h pkm ] && ln -s pk pkm || true
	[ ! -h pkc ] && ln -s pk pkc || true
	[ ! -h pku ] && ln -s pk pku || true
	[ ! -h pkl ] && ln -s pk pkl || true
	[ ! -h pks ] && ln -s pk pks || true
	[ ! -h pki ] && ln -s pk pki || true

clean:
	$(RM) pk  || true
	$(RM) pkm || true
	$(RM) pkc || true
	$(RM) pku || true
	$(RM) pkl || true
	$(RM) pks || true
	$(RM) pki || true
	$(RM) *~

diff:
	git diff | colordiff | less -R

test_pks:
	./pks vim
	
changelog:
	./tools/gitlog-to-changelog > doc/Changelog
	cat doc/Changelog.svn >> doc/Changelog
