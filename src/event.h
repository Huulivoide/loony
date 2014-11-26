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

/** What kind of event happened? */
typedef enum loonyevent_t
{
    LEVENT_KEYPRESS             /* a key was pressed */
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
