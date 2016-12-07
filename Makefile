CC=gcc
CFLAGS=-I -Wformat-security -DUNIX

all: console_utils.o file.o file_system.o folder.o f_pool.o hard_drive.o interface.o main.o memutils.o shell.o trace.o editor.o
	$(CC) -o abe console_utils.o file.o file_system.o folder.o f_pool.o hard_drive.o interface.o main.o memutils.o shell.o trace.o editor.o $(CFLAGS)