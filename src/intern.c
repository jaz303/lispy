#include "lispy/intern.h"

#include <stdlib.h>
#include "jazlib/common.h"

#define FREE_STRING(hsh, k) free((void*)k)

#include "jazlib/gen_hash_reset.h"
#define GEN_HASH_HASH_FUNC  hash_djb2
#define GEN_HASH_KEY_CMP    strcmp
#define GEN_HASH_KEY_COPY   gen_strcpy
#define GEN_HASH_KEY_FREE   FREE_STRING
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STATIC_INTERFACE(__intern_s2i, const char *, INTERN);
GEN_HASH_INIT(__intern_s2i, const char *, INTERN);

#include "jazlib/gen_hash_reset.h"
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STATIC_INTERFACE(__intern_i2s, INTERN, const char *);
GEN_HASH_INIT(__intern_i2s, INTERN, const char *);

void intern_table_init(intern_table_t *table) {
    table->size = 0;
    __intern_s2i_init(&table->string_to_intern);
    __intern_i2s_init(&table->intern_to_string);
}

INTERN intern_table_put(intern_table_t *table, const char *str) {
    INTERN value = intern_table_get_value(table, str);
    if (!value) {
        table->size++;
        __intern_s2i_put(table, str, table->size);
        return table->size;
    } else {
        return value;
    }
}

INTERN intern_table_get_value(intern_table_t *table, const char *str) {
    
}

const char* intern_table_get_str(intern_table_t *table, INTERN val) {
    
}
