#include "event.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

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

static LoonyEvent *loonyevent_create_keypress(lua_State *L)
{
    LoonyEvent *event;

    assert(L != NULL);

    event = malloc(sizeof(*event));
    if (!event) {
        return NULL;
    }

    event->type = LEVENT_KEYPRESS;
    event->data = malloc(1);
    if (!event->data) {
        free(event);
        return NULL;
    }

    *(char *)event->data = lua_tostring(L, -2)[0];
    return event;
}

/** Functions for creating events. */
static LoonyEvent *(* const event_functions[])(lua_State *L) =
{
    loonyevent_create_keypress
};

LoonyEvent *loonyevent_create_event(lua_State *L)
{
    int event_num;

    assert(L != NULL);

    event_num = lua_tonumber(L, -3);
    if (event_num < 0 || event_num >= LEVENT_NUM_EVENTS) {
        return NULL;
    } else {
        return event_functions[event_num](L);
    }
}
