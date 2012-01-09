#include "lispy/eval.h"
#include "lispy/binding.h"
#include "lispy/gc.h"
#include "lispy/intern.h"
#include "lispy/io.h"

#include <stdio.h>

// http://norvig.com/lispy.html
// http://norvig.com/lispy2.html

static void abort_error(env_t *env, const char *msg) {
    env->error = msg;
    longjmp(env->error_jmp, 0);
}

#define ENSURE_ARITY(lst, arity)            if (list_len(lst) != arity)     return kError
#define ENSURE_MIN_ARITY(lst, min_arity)    if (list_len(lst) < min_arity)  return kError
#define ENSURE_MAX_ARITY(lst, max_arity)    if (list_len(lst) > max_arity)  return kError

#define BUILTIN(name) static inline VALUE name(env_t *env, binding_t *binding, list_t *list)

BUILTIN(eval_quote) {
    ENSURE_ARITY(list, 2);
    return list_get(list, 1);
}

BUILTIN(eval_if) {
    if (list_len(list) == 3) {
        return VALUE_IS_TRUTHY(EVAL(list_get(list, 1)))
                ? EVAL(list_get(list, 2))
                : kNil;
    } else if (list_len(list) == 4) {
        return VALUE_IS_TRUTHY(EVAL(list_get(list, 1)))
                ? EVAL(list_get(list, 2))
                : EVAL(list_get(list, 3));
    }
    
    abort_error(env, "form `if` expects 2 or 3 args");
    return kUnreachable;
}

BUILTIN(eval_set) {
    ENSURE_ARITY(list, 3);
    
    VALUE ident = list_get(list, 1);
    if (!VALUE_IS_IDENT(ident)) {
        abort_error(env, "arg 1 of form `set!` must be an ident");
        return kUnreachable;
    }
    
    binding_t *source = binding_find(binding, IDENT(ident));
    if (source) {
        VALUE v = EVAL(list_get(list, 2));
        binding_set(source, IDENT(ident), v);
        return v;
    }
    
    abort_error(env, "can't set undefined identifier");
    return kUnreachable;
}

BUILTIN(eval_define) {
    ENSURE_ARITY(list, 3);
    
    VALUE ident = list_get(list, 1),
          value = list_get(list, 2);
    
    if (!VALUE_IS_IDENT(ident)) {
        return kError;
    }
    
    value = EVAL(value);
    binding_set(binding, IDENT(ident), value);
    
    return value;
}

BUILTIN(eval_lambda) {
    
    list_t *args = NULL;
    list_t *defn = NULL;
    
    if (list_len(list) == 3) {
        
        args = list_get(list, 1);
        defn = list_get(list, 2);
        
        if (!IS_LIST(args)) return kError;
        if (!IS_LIST(defn)) return kError;
        
        int i;
        for (i = 0; i < list_len(args); i++) {
            if (!VALUE_IS_IDENT(list_get(args, i))) {
                return kError;
            }
        }
        
    } else if (list_len(list) == 2) {
        
        args = NULL;
        defn = list_get(list, 1);
        
        if (!IS_LIST(defn)) return kError;
    
    } else {
        
        return kError;
    
    }
    
    return (VALUE) gc_alloc_lambda(&env->gc, binding, args, defn);
}

BUILTIN(eval_begin) {
    int i;
	VALUE out;
	for (i = 1; i < list_len(list); i++) {
        out = EVAL(list_get(list, i));
	}
	return out;
}

BUILTIN(eval_pprint) {
    pretty_print(env, EVAL(list_get(list, 1)), 0);
    return kNil;
}

VALUE eval_list(env_t *env, binding_t *binding, list_t *list) {
    
    if (list_len(list) == 0) {
        return kError;
    }
    
    /* special forms */
    
    VALUE head = list_get(list, 0);
    if (VALUE_IS_IDENT(head)) {
        switch (IDENT(head)) {
            case 1:     return eval_quote(env, binding, list);
            case 2:     return eval_if(env, binding, list);
            case 3:     return eval_set(env, binding, list);
            case 4:     return eval_define(env, binding, list);
            case 5:     return eval_lambda(env, binding, list);
            case 6:     return eval_begin(env, binding, list);
            case 7:     return eval_pprint(env, binding, list);
        }
    }
    
    /* and the rest */
    
    head = EVAL(head);
    
    if (IS_OBJECT(head)) {
        if (IS_A(head, TYPE_NATIVE_FN)) {
            if (list_len(list) - 1 == AS_NATIVE_FN(head)->arity) {
                return AS_NATIVE_FN(head)->fn(env, binding, list);
            }
        } else if (IS_A(head, TYPE_LAMBDA)) {
            lambda_t *lambda = AS_LAMBDA(head);
            
            if (lambda->arity != list_len(list) - 1) {
                return kError;
            }
            
            binding_t *new_binding = gc_alloc_binding(&env->gc, lambda->binding);
            
            int i;
            for (i = 0; i < lambda->arity; i++) {
                binding_set(new_binding,
                            IDENT(list_get(lambda->args, i)),
                            EVAL(list_get(list, i + 1)));
            }
            
            return eval_list(env, new_binding, lambda->defn);
        }
    }
    
    return kError;
    
}

VALUE eval(env_t *env, binding_t *binding, VALUE val) {
    return EVAL(val);
}
