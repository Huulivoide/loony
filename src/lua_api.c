#include "lua_api.h"

#include "lauxlib.h"

#include "inttrie.h"

/*
 * Adds a new global command.
 */
static int api_add_command(lua_State *L)
{
    const char *cmd = luaL_checkstring(L, 1);
    if (!lua_isfunction(L, 2)) {
        luaL_error(L, "loony.add_command: arg #2 must be a function, found %s",
                      lua_typename(L, lua_type(L, 2)));
    }

    /* ensure that the callback function is on top of the stack */
    lua_pushvalue(L, 2);
    const int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_getfield(L, LUA_REGISTRYINDEX, "loony_keybinds");
    IntTrie *trie = lua_touserdata(L, -1);
    IntTrieStatus status = inttrie_insert(trie, cmd, ref);
    switch (status) {
        case INTTRIE_OK:
            break;
        case INTTRIE_AMBIGUOUS:
            luaL_unref(L, LUA_REGISTRYINDEX, ref);
            luaL_error(L, "loony.add_command: ambiguous command %s", cmd);
            break;
        case INTTRIE_EXISTS:
            luaL_unref(L, LUA_REGISTRYINDEX, ref);
            luaL_error(L, "loony.add_command: command %s already exists", cmd);
        default:
            luaL_error(L, "loony.add_command: something unexpected happened");
            break;
    }
    return 0;
}

/*
 * API functions.
 */
static const luaL_Reg loonylib[] = {
    {"add_command", api_add_command},
    {NULL, NULL}
};

int loonyapi_open(lua_State *L)
{
    luaL_newlib(L, loonylib);
    lua_setglobal(L, "loony");

    /* global keybindings */
    lua_pushlightuserdata(L, inttrie_init());
    lua_setfield(L, LUA_REGISTRYINDEX, "loony_keybinds");

    return 0;
}
