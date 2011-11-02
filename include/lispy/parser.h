#ifndef PARSER_H
#define PARSER_H

#include "lispy/lexer.h"
#include "lispy/object.h"
#include "lispy/intern.h"

typedef struct parser {
	lexer_t			*lexer;
	intern_table_t  *intern;
	const char		*error;
} parser_t;

void	parser_init(parser_t *parser);
list_t*	parser_parse(parser_t *parser);

#endif
