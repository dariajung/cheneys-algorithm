#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* --------------------------------------------------------------- */
/*                   Cheney's Copying Algorithm                    */
/* --------------------------------------------------------------- */

/*  Assumptions: */
//  Same sized objects (though this is not a requirement)

/*  A heap object. */
typedef struct object {

    struct object * forwardee;
    unsigned char is_forwarded;

    /* Djikstra's Tricoloring

    black - Self and descendents visited
    grey - Self visited, but not descendents 
    white - Unvisited, at the end of tracing, considered garbage 

    */
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
