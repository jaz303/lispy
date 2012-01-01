#ifndef BINDING_H
#define BINDING_H

#include "lispy/lispy.h"

int         binding_init(binding_t *binding, binding_t *parent);
binding_t*	binding_find(binding_t *binding, INTERN key);
VALUE		binding_get(binding_t *binding, INTERN key);
void		binding_set(binding_t *binding, INTERN key, VALUE value);

#endif
