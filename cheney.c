#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define HEAP_SIZE 100

/* --------------------------------------------------------------- */
/*                   Cheney's Copying Algorithm                    */
/* --------------------------------------------------------------- */

static ROOTS[HEAP_SIZE]; 

/*  Assumptions: */
/* Same sized objects (though this is not a requirement) */

/*  A heap object. */
typedef struct object {

    struct object * forwardee;
    unsigned char is_forwarded;

    /* Djikstra's Tricoloring

    black - Self and descendents visited
    grey - Self visited, but not descendents 
    white - Unvisited, at the end of tracing, considered garbage 

    */

    /* Object on the heap is either an int, or a cons cell */
    union {
        int value;

        struct {
            struct heap_object *car;
            struct heap_object *cdr;
        };
    };

} OBJECT;

/* The heap itself, broken into two semi-spaces. */
typedef struct heap {
    SEMISPACE *from_space;
    SEMISPACE *to_space;

} HEAP;

/* A semispace of the heap. */
typedef struct semispace {
    intptr_t bottom;
    intptr_t top;
    intptr_t end;

} SEMISPACE;

/* Initializes the heap */
void init_heap();

/* Creates an object on the heap, does not set value */
OBJECT * create_heap_object();

OBJECT * create_integer(int value);

OBJECT * create_cons(OBJECT * car, OBJECT * cdr);

/* Creates something of type NIL, ie: when neither 
integer nor cons cell is created, but memory allocated.
Similar to C's (void *)0 */
OBJECT * create_nil();

/* Set forwarding address from from_space to to_space */
void forward_to(intptr_t address);

/* flip semispaces */
void flip_spaces();

/* trace from the roots */
void trace();

/* collected un-traced/white objects */
void collect();

/* Given a size of the object to allocate, finds a free spot in from_space */
intptr_t cheney_allocate(size_t size);

/* Copy contents of object from from_space to to_space; use memset? */
void copy(OBJECT * to, OBJECT *from, size_t size);

/* The collective action of copying an object from from_space to to_space */
/* and setting the forwarding pointer */
OBJECT * evacuate(OBJECT * obj);
