#include "lispy/io.h"
#include "lispy/intern.h"

#include <stdio.h>

static void indent(int indent) {
	int i;
	for (i = 0; i < indent; i++) fputs("  ", stdout);
}

void pretty_print(env_t *env, VALUE v, int i) {
    if (VALUE_IS_ERROR(v)) {
        printf("<error>\n");
    } else if (IS_LIST(v)) {
        printf("(\n");
		int j;
		for (j = 0; j < list_len(v); j++) {
            indent(i + 1); pretty_print(env, list_get(v, j), i + 1);
		}
		indent(i); printf(")\n");
    } else {
        if (VALUE_IS_INT(v)) {
    		printf("%lld\n", INTVAL(v));
    	} else if (VALUE_IS_BOOL(v)) {
    		printf("#%c\n", BOOLVAL(v) ? 't' : 'f');
        } else if (VALUE_IS_NIL(v)) {
            printf("#nil\n");
        } else if (VALUE_IS_IDENT(v)) {
            printf("%s\n", intern_table_get_str(&env->intern, IDENT(v)));
        } else if (VALUE_IS_ATOM(v)) {
            printf(":%s\n", intern_table_get_str(&env->intern, ATOM(v)));
        } else if (IS_STRING(v)) {
    		printf("\"%s\"\n", string_chars(v));
		} else {
            printf("<unknown %p>\n", v);
    	}
    }
}
