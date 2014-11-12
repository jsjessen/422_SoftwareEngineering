NAME = comp 


FILES = test.c \
		compare.c compare.h \
		input.c input.h \
		fdimage makefile \
		my_showblock \
		bug_showblock \
		kc_showblock.bin

OBJS = compare.o input.o

CC = gcc
DEBUG = -Wall -g -O0
FLAGS = $(DEBUG)

all:
	gcc -o $(NAME) $(DEBUG) tmp_comp.c compare.c input.c
