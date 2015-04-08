#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* --------------------------------------------------------------- */
/*                   Cheney's Copying Algorithm                    */
/* --------------------------------------------------------------- */

/*  Assumptions: */
//  Same sized objects (though this is not a requirement)


typedef struct object {

    struct object * forwardee;
    unsigned char is_forwarded;

    /* Djikstra's Tricoloring

    black - Self and descendents visited
    grey - Self visited, but not descendents 
    white - Unvisited, at the end of tracing, considered garbage 

    */
}

typedef struct heap {
    SEMISPACE *from_space;
    SEMISPACE *to_space;

} HEAP;

typedef struct semispace {
    intptr_t bottom;
    intptr_t top;
    intptr_t end;

} SEMISPACE;

void forward_to(intptr_t address);

void flip_spaces();

void trace();

void collect();

intptr_t cheney_allocate(size_t size);