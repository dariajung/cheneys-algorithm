# Makefile for test driver
CC=gcc
CFLAGS=-Wall -g

EXE=cheney
OBJS=cheney.o

HEADER=cheney.h

.PHONY: compile clean distclean 
compile: $(EXE)

$(EXE): $(OBJS)

$(OBJS): $(HEADER)

run: compile 
	./$(EXE)

cheney: cheney.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXE) $(OBJS) core* cheney

distclean: clean