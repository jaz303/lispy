#include "lispy/eval.h"
#include "lispy/binding.h"
#include "lispy/gc.h"

#include <stdio.h>

#define ENSURE_ARITY(lst, arity)            if (list_len(lst) != arity)     return kError
#define ENSURE_MIN_ARITY(lst, min_arity)    if (list_len(lst) < min_arity)  return kError
#define ENSURE_MAX_ARITY(lst, max_arity)    if (list_len(lst) > max_arity)  return kError

VALUE eval_list(env_t *env, binding_t *binding, list_t *list) {
    if (list->length > 0) {
        VALUE head = list_get(list, 0);
        if (VALUE_IS_IDENT(head)) {
            switch (IDENT(head)) {
                case 1: /* def */
                {
                    ENSURE_ARITY(list, 3);
                    VALUE ident = list_get(list, 1),
                          value = list_get(list, 2);
                    if (!VALUE_IS_IDENT(ident)) {
                        return kError;
                    } else {
                        binding_set(binding, IDENT(ident), value);
                    }
                    return value;
                }
                case 2: /* do */
                {
                    int i;
                	VALUE out;
                	for (i = 1; i < list_len(list); i++) {
                        out = EVAL(list_get(list, i));
                	}
                	return out;
                }
                case 3: /* if */
                {
                    if (list_len(list) == 3) {
                        return VALUE_IS_TRUTHY(EVAL(list_get(list, 1)))
                                ? EVAL(list_get(list, 2))
                                : kNil;
                    } else if (list_len(list) == 4) {
                        return VALUE_IS_TRUTHY(EVAL(list_get(list, 1)))
                                ? EVAL(list_get(list, 2))
                                : EVAL(list_get(list, 3));
                    } else {
                        return kError;
                    }
                }
                case 4: /* println */
                {
                    printf("println\n");
                    break;
                }
                case 5: /* quote */
                {
                    ENSURE_ARITY(list, 2);
                    return list_get(list, 1);
                }
                case 6: /* set */
                {
                    ENSURE_ARITY(list, 3);
                    
                    VALUE ident = list_get(list, 1);
                    if (!VALUE_IS_IDENT(ident)) {
                        return kError;
                    }
                    
                    binding_t *source = binding_find(binding, IDENT(ident));
                    if (source) {
                        VALUE v = list_get(list, 2);
                        binding_set(source, IDENT(ident), v);
                        return v;
                    } else {
                        return kError;
                    }
                }
                case 7: /* get */
                {
                    ENSURE_ARITY(list, 2);
                    
                    VALUE ident = list_get(list, 1);
                    if (!VALUE_IS_IDENT(ident)) {
                        return kError;
                    }
                    
                    return binding_lookup(binding, IDENT(ident));
                }
                case 8: /* first */
                {
                    ENSURE_ARITY(list, 2);
                    
                    VALUE arg = EVAL(list_get(list, 1));
                    if (!IS_LIST(arg))      return kError;
                    if (list_len(arg) < 1)  return kError;
                    
                    return list_get(arg, 0);
                }
                case 9: /* rest */
                {
                    ENSURE_ARITY(list, 2);
                    
                    VALUE arg = EVAL(list_get(list, 1));
                    if (!IS_LIST(arg))      return kError;
                    if (list_len(arg) < 1)  return kError;
                    
                    list_t *rest = gc_alloc_list(&env->gc, list_len(arg) - 1);
                    int i;
                    for (i = 1; i < list_len(arg); i++) {
                        list_set(rest, i - 1, list_get(arg, i));
                    }
                    
                    return (VALUE)rest;
                }
                default: /* something else */
                {
                    VALUE def = binding_lookup(binding, IDENT(head));
                    if (IS_OBJECT(def)) {
                        if (IS_A(def, TYPE_NATIVE_FN)) {
                            return AS_NATIVE_FN(def)->fn(env, binding, list);
                        }
                    }
                    
                    return kError;
                }
            }
        }
    }
    return kError;
}

VALUE eval(env_t *env, binding_t *binding, VALUE val) {
    return EVAL(val);
}
