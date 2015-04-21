#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cheney.h"

#define HEAP_SIZE 100

/* --------------------------------------------------------------- */
/*                   Cheney's Copying Algorithm                    */
/* --------------------------------------------------------------- */


/* Initializes the heap, takes a pointer to struct heap */
void init_heap(HEAP *heap);

/* Creates an OBJECT on the heap and returns a pointer to it,
neither value nor (cdr, car) are set. A "null" value? Unclear how
to represent. */
OBJECT * create_nil();

OBJECT * create_integer(int value);

OBJECT * create_cons(OBJECT * car, OBJECT * cdr);

int main() {
    return 0;
}

