#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cheney_data_structures.h"

#define HEAP_SIZE 100

static HEAP *heap;

/* ------------ HIDE FROM USER -------------- */ 

/*  Assumptions: */
/*  Same sized objects (though this is not a requirement) */

/* Cheney's Algorithm has an equivalence to the
Tricoloring Abstraction

black - Self and descendents visited
grey - Self visited, but not descendents; nodes in work list in tospace
white - Unvisited, at the end of tracing, considered garbage 

*/

/* Initializes the heap, takes a pointer to struct heap */
void init_heap(HEAP *heap) {
    void *temp_top, *temp_end;
    size_t space_size;

    temp_top = malloc(sizeof(OBJECT) * HEAP_SIZE);
    temp_end = temp_top + sizeof(OBJECT) * HEAP_SIZE;

    space_size = (temp_end - temp_top) / 2.0;

    heap->size_semi = space_size;
    heap->scan = NULL;
    heap->_free = temp_top;

    // from space is the initially used semi-heap
    heap->from_space.top = temp_top;
    heap->from_space.end = temp_top + space_size;

    // on "reserve" until from space no longer has memory
    heap->to_space.top = temp_top + space_size;
    heap->to_space.end = temp_end;
}

/* trace from the root */
/* Actually is this even necessary tho */
/* static void trace(); */

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
static void collect() {

}

static void * cheney_allocate(size_t size) {
    void * tmp;
    tmp = NULL;

    // also check if there is no room at all?

    if (heap->_free >= heap->from_space.end) {
        printf("Free pointer is past the from space semi heap's boundaries\n Calling collect.\n");
        // call collect, and then try allocating again
        collect();
        cheney_allocate(size);
    // otherwise, allocate heap object where free pointer points
    } else {
        tmp = heap->_free;
        heap->_free += size;
    }

    return tmp;
}

/* Set forwarding address from from_space to to_space */
static void forward_to(void * address);

/* flip semispaces */
static void flip_spaces();

/* Given a size of the object to allocate, finds a free spot in from_space */
/* Check where the free pointer is, and if it's >= the limit of a heap, need
    to call collector */
/* Does it make sense to pass the whole heap? */

/* Copy contents of object from from_space to to_space; use memcpy? */
static void copy(OBJECT * p);

/* Sets forwarding address. Will be called from copy */
static OBJECT * set_forwarding_address(OBJECT * obj, OBJECT * alloc_pointer);

