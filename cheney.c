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
    obj->_type = 0;
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
    obj->_type = 1;
    obj->car_forwarding = car;
    obj->cdr = cdr;

    return obj;
}

void test_correct_response() {
    OBJECT *test1, *test2, *test3, *test4, *test5, *test6;
    test1 = create_integer(7);
    test2 = create_integer(8);
    test3 = create_cons(test1, test2);
    test4 = create_cons(test3, NIL);
    test5 = create_integer(18);
    
    test6 = create_integer(2);

    assert(test6 == NULL);
}

// make sure all children are correctly found
void test_children() {
    OBJECT *test1, *test2, *test3, *test4;
    SListValue *tmp;
    SListEntry *list; // should this list be malloc'd?
    SListIterator *iter;

    tmp = NULL;

    list = malloc(sizeof(SListEntry));
    iter = malloc(sizeof(SListIterator));

    memset((void *)list, 0, sizeof(SListEntry));
    memset((void *)iter, 0, sizeof(SListIterator));

    slist_iterate(&list, iter);

    test1 = create_integer(7);
    test2 = create_integer(8);
    test3 = create_cons(test1, test2);
    test4 = create_cons(test3, NIL);

    /* 
        7
        8 
        cons cell [7, 8]
        cons cell [[7, 8], NIL]

        In total, there are 5 references,
        7, 8, [7, 8], [[_, _], NIL], NIL
    */

    children((void *)test4, list);

    printf("Length of list %d\n", slist_length(list));

    while (slist_iter_has_more(iter) > 0) {
        printf("hello\n");
        // returns data of node
        tmp = slist_iter_next(iter);
        if (tmp) {
            printf("Type %d\n", ((OBJECT *)tmp)->_type);

            if ((((OBJECT *)tmp)->_type) == 0) {
                printf("Integer value: %d\n", ((OBJECT *)tmp)->value);
            }
        }
    }

    slist_free(list);
    free(iter);

    printf("goodbye\n");

}

int main() {

    init_heap();

    //test_children();
    test_correct_response();

    cleanup();
    return 0;
}

