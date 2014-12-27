/**
 * @file lua_api.h
 * @author dreamyeyed
 *
 * Lua API for Loony. This file contains functions that allow the C core to
 * communicate with Lua scripts.
 */

#pragma once

#include "lua.h"

#include "window.h"

/**
 * Open the Loony API in the given Lua state.
 *
 * @param L pointer to a Lua state
 * @return 0 on success, non-zero otherwise.
 */
int loony_open_lua_api(lua_State *L);
