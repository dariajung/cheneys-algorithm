#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "cheney_data_structures.h"
#include "slist.h"

#define HEAP_SIZE 10

static HEAP *heap;
//OBJECT * root; // this needs to be figured out; a linked list or an array, idk

// head of root linked list
static SListEntry *root_list;
// iterator for root linked list
static SListIterator *root_iter;

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

    root_list = malloc(sizeof(SListEntry));
    root_iter = malloc(sizeof(SListIterator));

    memset((void *)root_list, 0, sizeof(SListEntry));
    memset((void *)root_iter, 0, sizeof(SListIterator));

    //root_list = NULL;
    slist_iterate(&root_list, root_iter);

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

    heap->memory_block_start = temp_top;
    heap->size_semi = half_space;

    // from space is the initially used semi-heap
    heap->from_space.top = temp_top;
    heap->from_space.end = temp_top + half_space;

    heap->scan = NULL;
    heap->_free = heap->from_space.top;

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

    printf("Flipping!\n");

    void * tmp;

    tmp = heap->from_space.top;

    heap->from_space.top = heap->to_space.top;
    heap->from_space.end = heap->to_space.top + heap->size_semi;

    heap->to_space.top = tmp;
    heap->to_space.end = tmp + heap->size_semi;
}

/* Set forwarding address from from_space to to_space */
static void forward_to(void * address, OBJECT *obj) {

    printf("Forwarding!\n");

    obj->is_forwarded = 1;
    obj->car_forwarding = address;
    obj->cdr = NULL; // is this necessary?
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

    printf("Copy!\n");

    void *new_address, *forwarding_address;
    new_address = NULL;
    forwarding_address = NULL;

    if (p->is_forwarded) {
        printf("Already FORWARDED\n");
        return (p->car_forwarding);
    } else {
        printf("Inside copy. Not forwarded yet!\n");
        // dest, src, size
        memcpy(heap->_free, p, sizeof(OBJECT));

        new_address = heap->_free;
        forwarding_address = heap->_free;
        forward_to(heap->_free, p);


        // if (p) {
        //     if (p->_type == 0) {
        //         printf("Before copy INT: %d\n", p->value);
        //     } else {
        //         printf("Before copy CONS\n");
        //     }

        //     printf("Is forwarded: %d\n", p->is_forwarded);
        // }

        // Make sure memcpy worked
        // if ((OBJECT *)heap->_free) {
        //     if (((OBJECT *)heap->_free)->_type == 0) {
        //         printf("At heap->_free INT: %d\n", ((OBJECT *)heap->_free)->value);
        //     }

        //     printf("Is forwarded: %d\n", ((OBJECT *)heap->_free)->is_forwarded);
        // }


        heap->_free += sizeof(OBJECT);
        return forwarding_address;
    }

    if (!forwarding_address) {
        printf("Forwarding address is NULL\n");
    }

    return forwarding_address;
}

// return a linked list of children of heap object
static SListEntry * children(void * obj, SListEntry * list) {
    SListEntry *tmp;
    tmp = NULL;
    printf("Inside children\n");

    // NULL object
    if (!obj) {
        return list;

    } else if (((OBJECT *)obj)->_type == 0) {
        printf("INTEGER %p\n", obj);
        tmp = slist_append(&list, obj);
        if (tmp) {
            printf("Data added %d\n", ((OBJECT *)(tmp->data))->value);
        }
        return list;

    } else if (((OBJECT *)obj)->_type == 1) {
        printf("CONS CELL: %p\n", obj);
        slist_append(&list, obj);
        printf("AFTER APPENDING TO LIST\n");
        children((void *)((OBJECT *)obj)->car_forwarding, list);
        children((void *)((OBJECT *)obj)->cdr, list);
    }

    return list;

}


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
    printf("Collect called!\n");

    void *p, *tmp, *root_tmp;
    SListEntry *list, *root_list_tmp;
    SListIterator *iterator;

    list = malloc(sizeof(SListEntry));
    root_list_tmp = malloc(sizeof(SListEntry));

    iterator = malloc(sizeof(SListIterator));
 
    slist_iterate(&list, iterator);

    // set to start of to-space
    heap->_free = heap->to_space.top;
    heap->scan = heap->_free;

    printf("Free pointer: %p, Scan pointer: %p\n", heap->_free, heap->scan);

    // all root objects have been copied to to-space
    while (slist_iter_has_more(root_iter) > 0) {
        //printf("Copying roots\n");
        root_tmp = slist_iter_next(root_iter);
        if (root_tmp) {
            root_tmp = copy(root_tmp);
            slist_append(&root_list_tmp, root_tmp);
        }
    }

    // now we can set the new root list, and free memory of old root list
    slist_free(root_list);
    root_list = root_list_tmp;

    // re-intialize the root iterator
    slist_iterate(&root_list, root_iter);

    printf("Before scanning through semiheap\n");

    // DEBUG: something is going wrong here
    while (heap->scan <= heap->_free) {
        p = heap->scan;
        
        // children, aka anything reachable
        // p should only have children if CONS cell
        // clear out the list before passing it
        memset((void *)list, 0, sizeof(SListEntry));
        children(p, list);

        while (slist_iter_has_more(iterator) > 0) {
            // returns SListValue (ie: data) of node
            tmp = slist_iter_next(iterator);
            if ((OBJECT *)tmp) {
                tmp = copy(tmp);
            }
        }

        printf("Incrementing scan pointer\n");
        // increment scan pointer
        heap->scan += sizeof(OBJECT);
    }

    printf("After while loop\n");

    // now we are done with the list of children and the children iterator
    slist_free(list);
    free(iterator);

    // everything has been copied over so flip semi-spaces
    flip_spaces();

    printf("Collection done!\n");
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
        printf("Free pointer is past the from space semi heap's boundaries\nUser should call collect.\n");
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
    slist_free(root_list);
    free(root_iter);
    free(heap->memory_block_start);
    free(heap);
    printf("Cleanup\n");
}
