#include "lua_api.h"

#include <stdlib.h>

#include "lauxlib.h"

#include "event.h"
#include "hook.h"

/** Event names (visible to Lua). */
static const char * const EVENT_NAMES[] =
{
    "keypress"
};

/* Creates a new global hook.
 * Parameters: event type, condition, callback function. */
static int loonyapi_set_hook(lua_State *L)
{
    LoonyEvent *event;

    if (lua_gettop(L) < 3) {
        luaL_error(L, "set_hook requires 3 arguments");
    }

    luaL_argcheck(L, lua_isnumber(L, 1), 1,
                  "set_hook: arg #1 must be a number");
    luaL_argcheck(L, lua_isfunction(L, 3), 3,
                  "set_hook: arg #3 must be a function");

    event = loonyevent_create_event(L);
    if (event) {
        loonyhook_add_global(event, luaL_ref(L, 3));
    } else {
        luaL_error(L, "Couldn't create a new hook");
    }

    return 0;
}

/** Table of API functions. */
static const struct luaL_Reg api_functions[] =
{
    { "set_hook", loonyapi_set_hook },
    { NULL, NULL }
};

int loony_open_lua_api(lua_State *L)
{
    luaL_newlib(L, api_functions);
    lua_setglobal(L, "loony");
    return 0;
}
