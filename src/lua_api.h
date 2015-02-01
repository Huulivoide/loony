/*
 * @file lua_api.h
 * @author dreamyeyed
 *
 * Lua API for Loony.
 */

#pragma once

#include "lua.h"

/**
 * Open Lua API in the given state. The API functions will be in a global table
 * named "loony".
 *
 * @param L Lua state to open API in
 * @return 0 on success
 */
int loonyapi_open(lua_State *L);
