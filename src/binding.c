#include "lispy/binding.h"

#include <stdlib.h>
#include "jazlib/common.h"

#include "jazlib/gen_hash_reset.h"
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STATIC_INTERFACE(__binding_table, INTERN, VALUE);
GEN_HASH_INIT(__binding_table, INTERN, VALUE);

// int scope_init(scope_t *scope, scope_t *outer) {
//  __scope_table_init(&scope->table);
//  scope->outer = outer;
//     return 1;
// }
// 
// void scope_dealloc(scope_t *scope) {
//  __scope_table_dealloc(&scope->table);
// }

int binding_init(binding_t *binding, binding_t *parent) {
    binding->obj.type = TYPE_BINDING;
    __binding_table_init(&binding->table);
    binding->parent = parent;
    return 1;
}

binding_t* binding_find(binding_t *binding, INTERN key) {
	while (binding) {
		if (__binding_table_contains(&binding->table, key)) {
            break;
		}
		binding = binding->parent;
	}
	return binding;    
}

VALUE binding_get(binding_t *binding, INTERN key) {
	VALUE out;
	if (__binding_table_read(&binding->table, key, &out)) {
		return out;
	} else {
		return NULL;
	}    
}

void binding_set(binding_t *binding, INTERN key, VALUE value) {
    __binding_table_put(&binding->table, key, value);
}
