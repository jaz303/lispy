#include "lispy/lispy.h"

#include "lispy/env.h"
#include "lispy/repl.h"
#include "lispy/io.h"
#include "lispy/lexer.h"
#include "lispy/parser.h"
#include "lispy/eval.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    env_t env;
    env_init(&env);
    
    if (argc > 1) {
        if (setjmp(env.error_jmp)) {
            printf("Error: %s\n", env.error);
        } else {
            
            VALUE out;

            int i;
            for (i = 1; i < argc; i++) {
                FILE *file = fopen(argv[1], "r");
                if (file) {

                    fseek(file, 0, SEEK_END);
                    long sz = ftell(file);
                    char *source = malloc(sz + 1);
                    fseek(file, 0, SEEK_SET);
                    fread(source, 1, sz, file);
                    source[sz] = 0;
                    fclose(file);

                    lexer_t lexer;
                    parser_t parser;

                    lexer_init(&lexer);
                    lexer_reset(&lexer, source);

                    parser_init(&parser, &lexer, &env);

                    VALUE value = parser_parse(&parser);
                    if (IS_LIST(value)) {
                        out = eval(&env, &env.binding, value);
                    } else if (value != NULL) {
                        printf("error: top level object must be list\n");
                        exit(1);
                    } else {
                        printf("parse error: %s\n", parser.error);
                        exit(1);
                    }

                    free(source);

                }
            }
            
        }
    
    } else {
        repl_run(&env);
    }
    
    return 0;

}