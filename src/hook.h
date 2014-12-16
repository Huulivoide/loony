/**
 * @file hook.h
 * @author dreamyeyed
 *
 * Hooks are used to control Loony from Lua. A hook consists of two parts:
 * a condition (in this case, a LoonyEvent) and a callback function. Lua has
 * a reference system, which allows us to refer to functions (or any other
 * variables) using an integer.
 *
 * When an event happens, Loony checks if there is a hook with that event. If
 * so, the function associated with that event is called.
 */

#pragma once

#include "event.h"

/** Possible errors from hooks */
typedef enum LoonyHookError
{
    LHOOK_OK = 0,       /* no error happened */
    LHOOK_EXISTS,       /* a hook with the given event exists already */
    LHOOK_NOMEM         /* couldn't allocate memory for the hook */
} LoonyHookError;

/**
 * Adds a new global hook.
 *
 * @param event The event that activates the hook. Do *not* free it or its data
 *              pointer - the hook system does it automatically. Make sure it's
 *              NULL if there is no data!
 * @param fn a reference that gets the callback from Lua
 * @return LHOOK_OK or LHOOK_EXISTS
 */
LoonyHookError loonyhook_add_global(const LoonyEvent *event, int fn);

/**
 * Checks if there is a hook with the given event and returns its callback
 * function.
 *
 * @param event the event that happened
 * @return a reference for Lua's reference system or LUA_NOREF if there's no
 *         hook with the given event
 */
int loonyhook_get_global(const LoonyEvent *event);
