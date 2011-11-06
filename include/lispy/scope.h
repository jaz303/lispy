#ifndef SCOPE_H
#define SCOPE_H

#include "lispy/global.h"

#include "jazlib/gen_hash_reset.h"
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STORAGE(__scope_table, INTERN, VALUE);

typedef struct scope scope_t;
struct scope {
	scope_t			*outer;
	__scope_table_t	table;
};

void		scope_init(scope_t *scope, scope_t *outer);
void		scope_dealloc(scope_t *scope);
scope_t*	scope_find(scope_t *scope, INTERN key);
VALUE		scope_get(scope_t *scope, INTERN key);
void		scope_set(scope_t *scope, INTERN key, VALUE value);

#endif
