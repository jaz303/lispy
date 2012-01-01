#ifndef EVAL_H
#define EVAL_H

#include "lispy/lispy.h"

VALUE eval(env_t *env, binding_t *binding, VALUE val);

#endif