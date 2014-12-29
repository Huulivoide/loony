#include "hook.h"

#include <assert.h>
#include <stdlib.h>

#include "lauxlib.h" // LUA_NOREF

/** Represents a hook. */
typedef struct LoonyHook
{
    /** the event that activates this hook */
    const LoonyEvent *event;
    /** a number that allows us to get the callback function from Lua */
    int fn;
} LoonyHook;

/** Array of global hooks. */
static LoonyHook *global_hooks;
/** Number of global hooks. */
static size_t num_hooks;
/** Size of the global hook array. */
static size_t hooks_size;

/* Used for sorting and searching the hook array. */
static int compare_hooks(const void *a, const void *b)
{
    const LoonyHook *hook_a = a;
    const LoonyHook *hook_b = b;
    return loonyevent_cmp(hook_a->event, hook_b->event);
}

LoonyHookError loonyhook_add_global(const LoonyEvent *event, int fn)
{
    /* Create the array if it doesn't exist yet */
    if (!global_hooks) {
        global_hooks = malloc(sizeof(*global_hooks) * 16);
        if (!global_hooks) {
            return LHOOK_NOMEM;
        }
        num_hooks = 0;
        hooks_size = 16;
    }

    /* Resize array if it's full */
    if (num_hooks == hooks_size) {
        global_hooks = realloc(global_hooks,
                               sizeof(*global_hooks) * hooks_size * 2);
        if (!global_hooks) {
            return LHOOK_NOMEM;
        }
        hooks_size *= 2;
    }

    global_hooks[num_hooks].event = event;
    global_hooks[num_hooks].fn = fn;
    ++num_hooks;
    /* TODO: use something more efficient */
    qsort(global_hooks, num_hooks, sizeof(*global_hooks), compare_hooks);
    return LHOOK_OK;
}

int loonyhook_get_global(const LoonyEvent *event)
{
    /* There's probably a better way to do this */
    LoonyHook dummy;
    const LoonyHook *hook;

    /* No hooks at all? */
    if (!global_hooks) {
        return LUA_NOREF;
    }

    dummy.event = event;
    dummy.fn = 0;
    hook = bsearch(&dummy, global_hooks, num_hooks, sizeof(*global_hooks),
                   compare_hooks);
    if (hook) {
        return hook->fn;
    } else {
        return LUA_NOREF;
    }
}
