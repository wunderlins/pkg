# GOAL: compile only platform specific functionality to keep it small

CC      = gcc
CCFLAGS = -Wall
RM      = rm
#	os      = $(shell uname -o 2>/dev/null || echo 0)

#CCFLAGS += -D OpenBSD
#CCFLAGS += -D Debian

ifeq ($(shell uname -o),GNU/Linux)
	OS = Debian
else 
	OS = OpenBSD
endif

CCFLAGS += -D $(OS)

all:
	@echo $(OS)
	$(CC) $(CCFLAGS) -o pk pk.c
	[ ! -h pkm ] && ln -s pk pkm || true
	[ ! -h pkc ] && ln -s pk pkc || true
	[ ! -h pku ] && ln -s pk pku || true
	[ ! -h pkl ] && ln -s pk pkl || true
	[ ! -h pks ] && ln -s pk pks || true
	[ ! -h pki ] && ln -s pk pki || true

clean:
	$(RM) pk || true
	$(RM) pk{c,h,i,l,p,r,s,u} || true
