/*
 * @file lua_api.h
 * @author dreamyeyed
 *
 * Lua API for Loony.
 */

#pragma once

#include "lua.h"

#include "textbuf.h"
#include "window.h"

/** Status messages from API functions. */
typedef enum LoonyApiStatus
{
    LOONYAPI_OK,                // no problems
    LOONYAPI_ERROR,             // generic error
    LOONYAPI_PARTIAL_MATCH,     // given command is prefix of bound key sequence
    LOONYAPI_NOT_FOUND,         // no such command exists
    LOONYAPI_UNEXPECTED         // something unexpected happened
} LoonyApiStatus;

/**
 * Open Lua API in the given state. The API functions will be in a global table
 * named "loony".
 *
 * @param L Lua state to open API in
 * @return 0 on success
 */
int loonyapi_open(lua_State *L);

/**
 * If command cmd exists, pushes the associated callback function on the Lua
 * stack.
 *
 * @param L Lua state
 * @param cmd command to check
 * @return LOONYAPI_OK if command exists, LOONYAPI_PARTIAL_MATCH if cmd is
 * prefix of an existing command, LOONYAPI_NOT_FOUND otherwise
 */
int loonyapi_get_callback(lua_State *L, const char *cmd);

/**
 * Pushes a LoonyWindow on the Lua stack.
 *
 * @param L
 * @param buf buffer to push on stack
 * @return LOONYAPI_OK on success, LOONYAPI_ERROR otherwise
 */
LoonyApiStatus loonyapi_push_loonywin(lua_State *L, LoonyWindow *win);
