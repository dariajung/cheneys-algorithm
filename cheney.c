#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cheney.h"

#define NIL NULL

/* --------------------------------------------------------------- */
/*                   Cheney's Copying Algorithm                    */
/* --------------------------------------------------------------- */

/* Creates an OBJECT on the heap and returns a pointer to it,
neither value nor (cdr, car) are set. A "null" value? Unclear how
to represent. 
OBJECT * create_nil() {
    OBJECT * obj;
    obj = (OBJECT *) cheney_allocate(sizeof(OBJECT));

    if (!obj) {
        printf("cheney_allocate failed\n");
        return NULL;
    }

    obj->is_forwarded = 0;
    obj->type = NIL;

    return obj;
}

*/

OBJECT * create_integer(int value) {
    OBJECT * obj;
    obj = (OBJECT *) cheney_allocate(sizeof(OBJECT));

    // allocate failed
    if (!obj) {
        printf("cheney_allocate failed\n");
        return NULL;
    }

    obj->is_forwarded = 0;
    obj->type = INT;
    obj->value = value;

    return obj;
}

OBJECT * create_cons(OBJECT * car, OBJECT * cdr) {
    OBJECT * obj;
    obj = (OBJECT *) cheney_allocate(sizeof(OBJECT));

    // allocate failed
    if (!obj) {
        printf("cheney_allocate failed\n");
        return NULL;
    }

    obj->is_forwarded = 0;
    obj->type = CONS;
    obj->car_forwarding = car;
    obj->cdr = cdr;

    return obj;
}

int main() {
    init_heap();

    cleanup();
    return 0;
}

