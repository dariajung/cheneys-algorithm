#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ------------ HIDE FROM USER -------------- */ 

/*  Assumptions: */
/*  Same sized objects (though this is not a requirement) */

/* Cheney's Algorithm has an equivalence to the
Tricoloring Abstraction

black - Self and descendents visited
grey - Self visited, but not descendents; nodes in work list in tospace
white - Unvisited, at the end of tracing, considered garbage 

*/

/*  A heap object. */
typedef struct object {

    /* 0 for unforwarded, 1 for forwarded */
    unsigned char is_forwarded;

    /* 0 for integer, 1 for cons */
    unsigned char type;

    /* Object on the heap is either an int, or a cons cell */
    union {
        int value;

        struct {

            /* 

            Any slot in a fromspace object can be used as 
            a forwarding address, so we can store the forwarding pointer
            in either *car or *cdr because its data has been copied somewhere else

            */

            struct object *car_forwarding;
            struct object *cdr;
        };
    };

} OBJECT;

OBJECT * root;

/* A semispace of the heap. */
typedef struct semispace {
    intptr_t top;
    intptr_t end;

} SEMISPACE;

/* The heap itself, broken into two semi-spaces. */
typedef struct heap {
    SEMISPACE from_space;
    SEMISPACE to_space;

    /* 
        Pointer used to linearly step through to-space objects
        A first in, first out queue of work list objects / grey nodes
        Trace a node's children only when the scan pointer reaches it in the queue
        After an object is copied from from-space to to-space, increment scan pointer
     */
    OBJECT * scan;

    /* The allocation pointer, points to the next free spot */
    OBJECT * _free;

    /* size of semispace in bytes */
    size_t size_semi;

    /* NOTE: When scan pointer reaches _free pointer, all objects from
    from-space have been copied */

} HEAP;

/* Creates an object on the heap, does not set value */
OBJECT * create_heap_object();

/* Set forwarding address from from_space to to_space */
static void forward_to(intptr_t address);

/* flip semispaces */
static void flip_spaces();

/* trace from the root */
static void trace();

/* collected un-traced/white objects */
/* collect pseudocode:
        from_space, to_space = to_space, from_space
        _free = to_space.top
        _end = _free + size_semi
        root = copy(root)
        while scan < _free:
            for p in children(scan):
                p = copy(p)
            scan += sizeof(scan);
*/
static void collect();

/* Given a size of the object to allocate, finds a free spot in from_space */
static intptr_t cheney_allocate(size_t size);

/* Copy contents of object from from_space to to_space; use memcpy? */
static void copy(OBJECT * p);

/* Sets forwarding address. Will be called from copy */
static OBJECT * set_forwarding_address(OBJECT * obj, OBJECT * alloc_pointer);

/* ---------- END HIDE FROM USER -------------- */