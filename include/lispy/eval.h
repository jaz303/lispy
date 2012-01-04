#ifndef EVAL_H
#define EVAL_H

#include "lispy/lispy.h"
#include "lispy/binding.h"

#define EVAL3(_e, _b, _v) \
    (IS_LIST(_v) \
        ? (eval_list(_e, _b, AS_LIST(_v))) \
        : (VALUE_IS_IDENT(_v) \
            ? (binding_lookup(_b, IDENT(_v))) \
            : (_v) \
        ) \
    )

#define EVAL(val) EVAL3(env, binding, val)

VALUE eval(env_t *env, binding_t *binding, VALUE val);
VALUE eval_list(env_t *env, binding_t *binding, list_t *list);

#endif