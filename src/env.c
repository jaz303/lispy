#include "lispy/env.h"

#include "lispy/binding.h"
#include "lispy/intern.h"
#include "lispy/native.h"
#include "lispy/gc.h"

static void add_native(env_t *env, const char *name, native_fn *fn) {
    binding_set(&env->binding,
                intern_table_put(&env->intern, name),
                gc_alloc_native_fn(&env->gc, fn));
}

int env_init(env_t *env) {
    
    if (!intern_table_init(&env->intern)) {
        return 0;
    }
    
    /* add builtin names to symbol table first so we know what their interned values will be */
    
    intern_table_put(&env->intern, "def");          // 1
    intern_table_put(&env->intern, "do");           // 2
    intern_table_put(&env->intern, "if");           // 3
    intern_table_put(&env->intern, "println");      // 4
    intern_table_put(&env->intern, "quote");        // 5
    intern_table_put(&env->intern, "set");          // 6
    intern_table_put(&env->intern, "get");          // 7
    intern_table_put(&env->intern, "first");        // 8
    intern_table_put(&env->intern, "rest");         // 9
    
    env->gc.root = NULL;
    env->gc.head = NULL;
    
    if (!binding_init(&env->binding, NULL)) {
        return 0;
    }
    
    add_native(env, "+",                native_plus);
    
    
    
    // +
	// -
	// /
	// *
	// pow
	// exp
	// eq
	// neq
    
    return 1;
}
