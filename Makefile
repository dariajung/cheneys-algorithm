C  = gcc
CXX = g++

CFLAGS   = -g -Wall  $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g

.PHONY: cheney slist
# default: cheney

# header dependency
# cheney.o: cheney.c

.PHONY: all

all: cheney

cheney: 
	gcc -Wall -o cheney cheney.c slist.c

# slist:
# 	gcc -Wall -o slist slist.c

clean:
	rm -f *.o *~ a.out core cheney slist