#ifndef IDENT_TABLE_H
#define IDENT_TABLE_H

#include "lispy/global.h"

#include "jazlib/gen_hash_reset.h"
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STORAGE(__intern_s2i, const char *, INTERN);

#include "jazlib/gen_hash_reset.h"
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STORAGE(__intern_i2s, INTERN, const char *);

typedef struct intern_table {
    size_t          size;
    __intern_s2i_t  string_to_intern;
    __intern_i2s_t  intern_to_string;
} intern_table_t;

void        intern_table_init(intern_table_t *table);
INTERN      intern_table_put(intern_table_t *table, const char *str);
INTERN      intern_table_get_value(intern_table_t *table, const char *str);
const char* intern_table_get_str(intern_table_t *table, INTERN val);

#endif