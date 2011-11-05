#ifndef SCOPE_H
#define SCOPE_H

#include "lispy/global.h"

#include "jazlib/gen_hash_reset.h"
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STORAGE(__scope_table, const char *, VALUE);

typedef struct scope scope_t;
struct scope {
	scope_t			*parent;
	__scope_table_t	table;
};

void		scope_init(scope_t *scope, scope_t *parent);
void		scope_dealloc(scope_t *scope);
VALUE		scope_find(scope_t *scope, const char *key);
VALUE		scope_get(scope_t *scope, const char *key);
void		scope_set(scope_t *scope, const char *key, VALUE value);

#endif
