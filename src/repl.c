#include "lispy/repl.h"

#include <stdio.h>
#include <stdlib.h>

#include "lispy/lexer.h"
#include "lispy/intern.h"
#include "lispy/parser.h"
#include "lispy/eval.h"

static void indent(int indent) {
	int i;
	for (i = 0; i < indent; i++) fputs("  ", stdout);
}

static void pretty_print(env_t *env, VALUE v, int i);
static void pretty_print(env_t *env, VALUE v, int i) {
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

void repl_run(env_t *env) {
    
    char buffer[1024];
    
    lexer_t lexer;
    parser_t parser;
    
    lexer_init(&lexer);
    
    do {
        fputs("> ", stdout);
        fgets(buffer, 1024, stdin);
        if (feof(stdin)) {
            fputs("\nEOF\n", stdout);
            break;
        }
        
        lexer_reset(&lexer, buffer);
        parser_init(&parser, &lexer, env);
        
        list_t *list = parser_parse(&parser);
        
        if (list) {
            pretty_print(env, eval(env, &env->binding, list), 0);
        } else {
            printf("parse error: %s\n", parser.error);
        }

    } while (1);
    
}
