#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "cheney_data_structures.h"
#include "slist.h"

#define HEAP_SIZE 100

static HEAP *heap;
OBJECT * root; // this needs to be figured out; a linked list or an array, idk

/*  Assumptions: */
/*  Same sized objects (though this is not a requirement) */

/* Cheney's Algorithm has an equivalence to the
Tricoloring Abstraction

black - Self and descendents visited
grey - Self visited, but not descendents; nodes in work list in tospace
white - Unvisited, at the end of tracing, considered garbage 

*/

/* Initializes the heap, takes a pointer to struct heap */
void init_heap() {
    printf("Initializing heap.\n");

    heap = malloc(sizeof(HEAP));

    void *temp_top, *temp_end;
    size_t total_space, half_space;

    total_space = sizeof(OBJECT) * HEAP_SIZE;

    temp_top = malloc(total_space);
    memset(temp_top, 0, total_space);
    temp_end = temp_top + total_space;

    half_space = (temp_end - temp_top) / 2.0;
    // sanity check
    assert(half_space == ((sizeof(OBJECT) * HEAP_SIZE) / 2.0 ));

    heap->memory_block = temp_top;
    heap->size_semi = half_space;
    heap->scan = NULL;
    heap->_free = NULL;

    // from space is the initially used semi-heap
    heap->from_space.top = temp_top;
    heap->from_space.end = temp_top + half_space;

    // sanity check
    assert(heap->from_space.end - heap->from_space.top == half_space);

    // on "reserve" until from space no longer has memory
    heap->to_space.top = temp_top + half_space;
    heap->to_space.end = temp_end;

    // sanity check
    assert(heap->to_space.end - heap->to_space.top == half_space);

    printf("Heap initialized.\n");
}

/* flip semispaces */
static void flip_spaces() {
    void * tmp;

    tmp = heap->from_space.top;

    heap->from_space.top = heap->to_space.top;
    heap->from_space.end = heap->to_space.top + heap->size_semi;

    heap->to_space.top = tmp;
    heap->to_space.end = tmp + heap->size_semi;
}

/* Set forwarding address from from_space to to_space */
static void forward_to(void * address, OBJECT *obj) {
    obj->is_forwarded = 1;
    obj->car_forwarding = address;
}

/* Copy contents of object from from_space to to_space; use memcpy? */
/* Copy pseudocode:
    if is_forwarded(p):
        return get_forwarding_pointer(p)
    else:
        memcpy(p, alloc_pointer)
        new_address = alloc_pointer
        forwarding_address = alloc_pointer
        set_forwarding_address(p, alloc_pointer)
        alloc_pointer = alloc_pointer + size(p)
        return forwarding_address
*/
static void * copy(OBJECT * p) {
    void *new_address, *forwarding_address;
    new_address = NULL;
    forwarding_address = NULL;

    if (p->is_forwarded) {
        return (p->car_forwarding);
    } else {
        // dest, src, size
        memcpy(heap->_free, p, sizeof(OBJECT));
        new_address = heap->_free;
        forwarding_address = heap->_free;
        forward_to(heap->_free, p);
        heap->_free += sizeof(OBJECT);
        return forwarding_address;
    }

    return forwarding_address;
}

// linked list of children


/* USER should call collect */
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
    void * p;

    heap->_free = heap->to_space.top;
    heap->scan = heap->_free;
    root = copy(root);
    while (heap->scan < heap->_free) {
        for (p = heap->scan; p <= (heap->to_space.end); p += sizeof(OBJECT)) {
            // if object there exists, copy it over
            if ((OBJECT *)p) {
                p = copy(p);
            }
        }
    }

    // everything has been copied over so flip semi-spaces
    flip_spaces();
}

/* 
    Given a size of the object to allocate, finds a free spot in from_space 

    Check where the free pointer is, and if it's >= the limit of a heap, throw an error.
    For purposes of functional hardware compiler project, user should be able to call
    the garbage collector at will 

*/
static void * cheney_allocate(size_t size) {
    void * tmp;
    tmp = NULL;

    // also check if there is no room at all?

    if (heap->_free >= heap->from_space.end) {
        printf("Free pointer is past the from space semi heap's boundaries\n User should call collect.\n");
        // call collect, and then try allocating again
        // check if there is no room even after collection
        // otherwise, allocate heap object where free pointer points
        return NULL;
    } else {
        tmp = heap->_free;
        heap->_free += size;
    }

    return tmp;
}

static void cleanup() {
    free(heap->memory_block);
    free(heap);
    printf("Allocated memory freed\n");
}
