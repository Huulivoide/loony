#include "event.h"

#include <assert.h>
#include <stddef.h>

static int levent_keypress_cmp(const LoonyEvent *a, const LoonyEvent *b)
{
    char *ac, *bc;

    assert(a != NULL && b != NULL);
    assert(a->type == LEVENT_KEYPRESS && b->type == LEVENT_KEYPRESS);

    ac = a->data;
    bc = b->data;

    if (*ac < *bc) {
        return -1;
    } else if (*ac > *bc) {
        return 1;
    } else {
        return 0;
    }
}

/** Functions for comparing events of a certain type. */
static int (* const cmp_functions[])(const LoonyEvent *, const LoonyEvent *) =
{
    levent_keypress_cmp
};

int loonyevent_cmp(const LoonyEvent *a, const LoonyEvent *b)
{
    if (a->type < b->type) {
        return -1;
    } else if (a->type > b->type) {
        return 1;
    } else {
        return cmp_functions[a->type](a, b);
    }
}
