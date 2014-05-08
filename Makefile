# GOAL: compile only platform specific functionality to keep it small

CC      = gcc
CCFLAGS = -Wall

all:
	$(CC) $(CCFLAGS) -o pk pk.c
