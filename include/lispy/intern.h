#ifndef INTERN_H
#define INTERN_H

#include "lispy/lispy.h"

int         intern_table_init(intern_table_t *table);
INTERN      intern_table_put(intern_table_t *table, const char *str);
INTERN      intern_table_get_value(intern_table_t *table, const char *str);
const char* intern_table_get_str(intern_table_t *table, INTERN val);

#endif