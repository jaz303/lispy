#include "lispy/repl.h"

#include <stdio.h>
#include <stdlib.h>

#include "lispy/lexer.h"
#include "lispy/intern.h"
#include "lispy/parser.h"
#include "lispy/eval.h"
#include "lispy/io.h"

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
        
        VALUE value = parser_parse(&parser);
        if (value != kError) {
            if (setjmp(env->error_jmp)) {
                printf("Error: %s\n", env->error);
            } else {
                pretty_print(env, eval(env, &env->binding, value), 0);
            }
        } else {
            printf("parse error: %s\n", parser.error);
        }

    } while (1);
    
}
