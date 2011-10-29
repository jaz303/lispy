#ifndef PARSER_H
#define PARSER_H

#include "lispy/ast.h"

typedef struct parser {
	const char		*source;
	const char		*error;
} parser_t;

void		parser_init(parser_t *parser, const char *source);
VALUE	parser_parse(parser_t *parser);

#endif
