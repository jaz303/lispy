#include "lispy/env.h"

#include "lispy/binding.h"
#include "lispy/intern.h"

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
    
    env->gc.root = NULL;
    env->gc.head = NULL;
    
    if (!binding_init(&env->binding, NULL)) {
        return 0;
    }
    
    return 1;
}
