#include "lispy/intern.h"

#include "jazlib/common.h"
#include <stdlib.h>

#include "jazlib/gen_hash_reset.h"
#define GEN_HASH_HASH_FUNC  hash_djb2
#define GEN_HASH_KEY_CMP    strcmp
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STATIC_INTERFACE(__intern_s2i, const char *, INTERN);
GEN_HASH_INIT(__intern_s2i, const char *, INTERN);

#define DIE_HORRIBLY()

int intern_table_init(intern_table_t *table) {
    table->size = 0;
    table->strings_sz = 16;
    table->strings = malloc(sizeof(char*) * table->strings_sz);
    if (!table->strings) {
        return 0;
    }
    table->strings[0] = NULL;
    __intern_s2i_init(&table->string_to_intern);
    return 1;
}

INTERN intern_table_put(intern_table_t *table, const char *str) {
    INTERN value = intern_table_get_value(table, str);
    if (value == 0) {
        char *copy = malloc(strlen(str) + 1);
        if (!copy) {
            DIE_HORRIBLY();
        }
        strcpy(copy, str);
        table->size++;
        if (table->strings_sz <= table->size) {
            table->strings_sz *= 2;
            table->strings = realloc(table->strings, sizeof(char*) * table->strings_sz);
            if (!table->strings) {
                DIE_HORRIBLY();
            }
        }
        table->strings[table->size] = copy;
        __intern_s2i_put(&table->string_to_intern, copy, table->size);
        return table->size;
    } else {
        return value;
    }
}

INTERN intern_table_get_value(intern_table_t *table, const char *str) {
	INTERN out;
    if (__intern_s2i_read(&table->string_to_intern, str, &out)) {
    	return out;
    } else {
    	return 0;
    }
}

const char* intern_table_get_str(intern_table_t *table, INTERN val) {
    return (val <= table->size) ? (table->strings[val]) : NULL;
}
