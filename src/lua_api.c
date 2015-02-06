#include "lua_api.h"

#include "lauxlib.h"

#include "inttrie.h"

/*
 * A wrapper around a LoonyWindow.
 */
typedef struct LuaLoonyWindow
{
    LoonyWindow *win;
} LuaLoonyWindow;

static int lw_move(lua_State *L)
{
    LuaLoonyWindow *lwin = luaL_checkudata(L, 1, "loony.window");
    loonywin_move_cursor(lwin->win, luaL_checkint(L, 2), luaL_checkint(L, 3));
    return 0;
}

static int lw_set_statusbar(lua_State *L)
{
    LuaLoonyWindow *lwin = luaL_checkudata(L, 1, "loony.window");
    loonywin_set_statusbar(lwin->win, luaL_checkstring(L, 2));
    return 0;
}

/*
 * LoonyWindow methods available to Lua.
 */
static const luaL_Reg loonywinlib[] = {
    {"move", lw_move},
    {"set_statusbar", lw_set_statusbar},
    {NULL, NULL}
};

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

    /* window metatable */
    luaL_newmetatable(L, "loony.window");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, loonywinlib, 0);
    lua_pop(L, 1);

    /* global keybindings */
    lua_pushlightuserdata(L, inttrie_init());
    lua_setfield(L, LUA_REGISTRYINDEX, "loony_keybinds");

    return 0;
}

int loonyapi_get_callback(lua_State *L, const char *cmd)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "loony_keybinds");
    IntTrie *trie = lua_touserdata(L, -1);
    int ref;
    IntTrieStatus status = inttrie_find(trie, cmd, &ref);
    switch (status) {
        case INTTRIE_OK:
            lua_pushinteger(L, ref);
            lua_gettable(L, LUA_REGISTRYINDEX);
            return LOONYAPI_OK;
            break;
        case INTTRIE_PARTIAL_MATCH:
            return LOONYAPI_PARTIAL_MATCH;
            break;
        case INTTRIE_NOT_FOUND:
            return LOONYAPI_NOT_FOUND;
            break;
        default:
            return LOONYAPI_UNEXPECTED;
            break;
    }
}

LoonyApiStatus loonyapi_push_loonywin(lua_State *L, LoonyWindow *win)
{
    LuaLoonyWindow *lwin = lua_newuserdata(L, sizeof(*lwin));
    lwin->win = win;
    luaL_setmetatable(L, "loony.window");
    return LOONYAPI_OK;
}
