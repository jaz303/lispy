#ifndef PARSER_H
#define PARSER_H

#include "lispy/lispy.h"

int         parser_init(parser_t *parser, lexer_t *lexer, env_t *env);
VALUE       parser_parse(parser_t *parser);

#endif
