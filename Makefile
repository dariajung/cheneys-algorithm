C  = gcc
CXX = g++

CFLAGS   = -g -Wall  $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g

# .PHONY: default
# default: cheney

# header dependency
# cheney.o: cheney.c

.PHONY: all

all: cheney slist

cheney: 
	gcc -Wall -o cheney cheney.c

slist:
	gcc -Wall -o slist slist.c

clean:
	rm -f *.o *~ a.out core cheney slist