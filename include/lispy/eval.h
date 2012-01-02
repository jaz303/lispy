#ifndef EVAL_H
#define EVAL_H

#include "lispy/lispy.h"

#define EVAL(val) (IS_LIST(val) ? (eval_list(env, binding, AS_LIST(val))) : (val))

VALUE eval(env_t *env, binding_t *binding, VALUE val);
VALUE eval_list(env_t *env, binding_t *binding, list_t *list);

#endif