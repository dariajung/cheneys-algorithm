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

/* A semispace of the heap. */
typedef struct semispace {
    intptr_t top;
    intptr_t end;

} SEMISPACE;
