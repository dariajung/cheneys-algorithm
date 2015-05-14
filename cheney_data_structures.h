
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

    /* 0 for INT, 1 for CONS */
    unsigned char _type;

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

/* A semispace of the heap. */
typedef struct semispace {
    void * top;
    void * end;

} SEMISPACE;


/* The heap itself, broken into two semi-spaces. */
typedef struct heap {
    // only for cleaning up purposes to free the memory at cleanup
    void * memory_block_start;

    SEMISPACE from_space;
    SEMISPACE to_space;

    /* 
        Pointer used to linearly step through to-space objects
        A first in, first out queue of work list objects / grey nodes
        Trace a node's children only when the scan pointer reaches it in the queue
        After an object is copied from from-space to to-space, increment scan pointer
     */
    void * scan;

    /* The allocation pointer, points to the next free address/spot */
    void * _free;

    /* size of semispace in bytes */
    size_t size_semi;

    /* NOTE: When scan pointer reaches _free pointer, all objects from
    from-space have been copied */

} HEAP;
