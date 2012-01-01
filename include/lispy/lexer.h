#ifndef LEXER_H
#define LEXER_H

#include "lispy/lispy.h"

void            lexer_init(lexer_t *l);
void			lexer_reset(lexer_t *l, const char *text);
token_t			lexer_next(lexer_t *l);

token_t			lexer_current_token(lexer_t *l);
INT				lexer_current_int(lexer_t *l);
const char*		lexer_current_str(lexer_t *l);

#endif
