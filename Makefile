CC=gcc
CFLAGS=-I.

all: console_utils.o file.o file_system.o folder.o f_pool.o interface.o main.o memutils.o shell.o trace.o
	$(CC) -o abe console_utils.o file.o file_system.o folder.o f_pool.o interface.o main.o memutils.o shell.o trace.o $(CFLAGS)