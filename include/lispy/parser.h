#ifndef PARSER_H
#define PARSER_H

#include "lispy/lexer.h"
#include "lispy/object.h"

typedef struct parser {
	lexer_t			*lexer;
	const char		*error;
} parser_t;

void		parser_init(parser_t *parser, lexer_t *lexer);
list_t*	parser_parse(parser_t *parser);

#endif
