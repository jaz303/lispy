#include "lispy/scope.h"

#include <stdlib.h>
#include "jazlib/common.h"

#define FREE_STRING(hsh, k) free((void*)k)

#include "jazlib/gen_hash_reset.h"
#define GEN_HASH_HASH_FUNC  hash_djb2
#define GEN_HASH_KEY_CMP    strcmp
#define GEN_HASH_KEY_COPY   gen_strcpy
#define GEN_HASH_KEY_FREE   FREE_STRING
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STATIC_INTERFACE(__scope_table, const char *, VALUE);
GEN_HASH_INIT(__scope_table, const char *, VALUE);

void scope_init(scope_t *scope, scope_t *parent) {
	__scope_table_init(&scope->table);
	scope->parent = parent;
}

void scope_dealloc(scope_t *scope) {
	__scope_table_dealloc(&scope->table);
}

VALUE scope_find(scope_t *scope, const char *key) {
	while (scope) {
		VALUE out;
		if (__scope_table_read(&scope->table, key, &out)) {
			return out;
		} else {
			scope = scope->parent;
		}
	}
	return NULL;
}

VALUE scope_get(scope_t *scope, const char *key) {
	VALUE out;
	if (__scope_table_read(&scope->table, key, &out)) {
		return out;
	} else {
		return NULL;
	}
}

void scope_set(scope_t *scope, const char *key, VALUE value) {
	__scope_table_put(&scope->table, key, value);
}

