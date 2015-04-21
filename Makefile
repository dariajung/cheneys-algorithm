C  = gcc
CXX = g++

CFLAGS   = -g -Wall  $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g

.PHONY: default
default: cheney

# header dependency
cheney.o: cheney.c

.PHONY: clean
clean:
	rm -f *.o *~ a.out core cheney

.PHONY: all
all: clean default