#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cheney.h"

/* --------------------------------------------------------------- */
/*                   Cheney's Copying Algorithm                    */
/* --------------------------------------------------------------- */

OBJECT * root;

/* Creates an OBJECT on the heap and returns a pointer to it,
neither value nor (cdr, car) are set. A "null" value? Unclear how
to represent. */
OBJECT * create_nil();

OBJECT * create_integer(int value) {
    OBJECT * obj;
    obj = (OBJECT *) cheney_allocate(sizeof(OBJECT));

    // allocate failed
    if (!obj) {
        return NULL;
    }

    obj->is_forwarded = 0;
    obj->type = 0;
    obj->value = value;

    return obj;
}

OBJECT * create_cons(OBJECT * car, OBJECT * cdr) {

}

int main() {
    return 0;
}

