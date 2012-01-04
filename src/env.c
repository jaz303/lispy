#include "lispy/env.h"

#include "lispy/binding.h"
#include "lispy/intern.h"
#include "lispy/native.h"
#include "lispy/gc.h"

static void add_native(env_t *env, const char *name, size_t arity, native_fn *fn) {
    binding_set(&env->binding,
                intern_table_put(&env->intern, name),
                gc_alloc_native_fn(&env->gc, 2, fn));
}

int env_init(env_t *env) {
    
    if (!intern_table_init(&env->intern)) {
        return 0;
    }
    
    /* add builtin names to symbol table first so we know what their interned values will be */
    
    intern_table_put(&env->intern, "quote");        // 1
    intern_table_put(&env->intern, "if");           // 2
    intern_table_put(&env->intern, "set!");         // 3
    intern_table_put(&env->intern, "define");       // 4
    intern_table_put(&env->intern, "lambda");       // 5
    intern_table_put(&env->intern, "begin");        // 6
    
    env->gc.root = NULL;
    env->gc.head = NULL;
    
    if (!binding_init(&env->binding, NULL)) {
        return 0;
    }
    
    add_native(env, "+",                2, native_plus);
    
    
    
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
