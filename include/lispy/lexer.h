#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

#include "lispy/global.h"

typedef struct lexer {
	const char		*text;
	int				pos;
	INT				curr_int;
	char				*curr_str;
	size_t			curr_str_sz;
	token_t			token;
	int				line;
	const char		*error;
} lexer_t;

void				lexer_init(lexer_t *l, const char *text);
token_t			lexer_next(lexer_t *l);

INT				lexer_current_int(lexer_t *l);
const char*		lexer_current_str(lexer_t *l);

#endif // LEXER_H
