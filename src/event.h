/**
 * @file event.h
 * @author dreamyeyed
 *
 * Events are used for scripting support. When something interesting happens,
 * an event is generated. A script starts when a specific event happens.
 *
 * An events consists of a type and some more specific information depending on
 * the type. For example, if a key was pressed, we want to know what key it was.
 */

#pragma once

#include "lua.h"

/** What kind of event happened? */
typedef enum loonyevent_t
{
    LEVENT_KEYPRESS = 0,        /* a key was pressed */
    LEVENT_NUM_EVENTS           /* number of different events */
} loonyevent_t;

/** A struct that represents an event. */
typedef struct LoonyEvent
{
    /** type of the event */
    loonyevent_t type;
    /** more information about the event, depending on the type */
    void *data;
} LoonyEvent;

/**
 * Compares two events. The order is pretty much arbitrary, but it's required
 * so we can use binary search to search for a specific event.
 *
 * @param a first event to compare
 * @param b second event to compare
 * @return negative, 0 or positive (similar to strcmp)
 */
int loonyevent_cmp(const LoonyEvent *a, const LoonyEvent *b);

/**
 * Creates an event from the top elements of the Lua stack.
 *
 * @param L Lua virtual machine
 * @return dynamically allocated event or NULL in case of error
 */
LoonyEvent *loonyevent_create_event(lua_State *L);
