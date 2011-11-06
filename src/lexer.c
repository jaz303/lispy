#include "lispy/lexer.h"

#include <string.h>
#include <ctype.h>

#define curr()		(l->text[l->pos])
#define next()		(l->pos++)
#define back()		(l->pos--)
#define emit(t)		l->token = t; return t
#define errmit(msg)	l->error = msg; emit(T_ERROR)

void lexer_init(lexer_t *l, const char *text) {
	l->text			= text;
	l->pos			= 0;
	l->curr_int		= 0;
	l->curr_str_sz	= 64;
	l->curr_str		= calloc(l->curr_str_sz, sizeof(char));
	l->token		= T_ERROR;
	l->line			= 1;
	l->error		= NULL;
}

//
// string buffer

void lexer_grow_str(lexer_t *l, size_t len) {
	if (l->curr_str_sz < len) {
		while (l->curr_str_sz < len) l->curr_str_sz *= 2;
		l->curr_str = realloc(l->curr_str, l->curr_str_sz);
	}
}

void lexer_copy_str(lexer_t *l, size_t pos, size_t len) {
	lexer_grow_str(l, len + 1);
	memcpy(l->curr_str, l->text + pos, len);
	l->curr_str[len] = '\0';
}

//
//

static int hex_value(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return 0;
}

static int isoctdigit(char c) { return c >= '0' && c < '8'; }
static int isbindigit(char c) { return c == '0' || c == '1'; }

static int is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static int is_ident_start(char c) {
	if (c >= 'a' && c <= 'z') return 1;
	if (c >= 'A' && c <= 'Z') return 1;
	return strchr("<>=+-*/_", c) != NULL;
}

static int is_ident_body(char c) {
	return is_ident_start(c);
}

static int is_atom_start(char c) {
	return is_ident_start(c);
}

static int is_atom_body(char c) {
	return is_ident_body(c);
}

//
//

static token_t scan_number(lexer_t *l) {
	l->curr_int = 0;

	int mul = 1;
	if (curr() == '+') {
		next();
	} else if (curr() == '-') {
		mul = -1;
		next();
	}

	if (curr() == '0') {
		next();
		if (curr() == 'x') {
			next();
			if (isxdigit(curr())) {
				while (isxdigit(curr())) {
					l->curr_int = (l->curr_int << 4) + hex_value(curr());
					next();
				}
				emit(T_INT);
			} else {
				errmit("illegal hex literal");
			}
		} else if (curr() == 'b') {
			next();
			if (isbindigit(curr())) {
				while (isbindigit(curr())) {
					l->curr_int = (l->curr_int << 1) + (curr() - '0');
					next();
				}
				emit(T_INT);
			} else {
				errmit("illegal binary literal");
			}
		} else if (isoctdigit(curr())) {
			l->curr_int = curr() - 48;
			next();
			while (isoctdigit(curr())) {
				l->curr_int = (l->curr_int * 8) + (curr() - '0');
				next();
			}
			emit(T_INT);
		} else if (curr() == '.') {
			// TODO: parse float
		}
	} else if (isdigit(curr())) {
		while (isdigit(curr()) || curr() == '_') {
			if (curr() != '_') {
				l->curr_int = (l->curr_int * 10) + (curr() - '0');
			}
			next();
		}
		if (curr() == '.') {
			errmit("floats not supported yet");
		} else {
			l->curr_int *= mul;
			emit(T_INT);
		}
	}
	errmit("failed to parse number");
}

static token_t scan_string(lexer_t *l) {
	if (curr() != '"') { errmit("expected '\"'"); }
	next();

	int start = l->pos, len = 0, slash = 0;

	for (;;) {
		if (curr() == '\n') l->line++;
		if (curr() == 0) {
			errmit("unexpected EOF");
		} else if (slash) {
			len++;
			slash = 0;
			next();
		} else if (curr() == '"'){
			next();
			break;
		} else if (curr() == '\\') {
			slash = 1;
			next();
		} else {
			len++;
			next();
		}
	}

	lexer_grow_str(l, len + 1);

	int pos = 0;
	const char *curr = l->text + start;

	while (pos < len) {
		if (slash) {
			slash = 0;
			char out;
			switch (*curr) {
				case 'n': { out = '\n'; break; }
				case 'r': { out = '\r'; break; }
				case 't': { out = '\t'; break; }
				case '"': { out = '"';  break; }
				default:  { errmit("illegal escape sequence"); }
			}
			l->curr_str[pos++] = out;
		} else if (*curr == '\\'){
			slash = 1;
		} else {
			l->curr_str[pos++] = *curr;
		}
		curr++;
	}

	l->curr_str[len] = '\0';

	emit(T_STRING);

}

static token_t scan_boolean(lexer_t *l) {
	if (curr() != '#') { emit(T_ERROR); }
	next();
	if (curr() == 't') { next(); emit(T_TRUE); }
	if (curr() == 'f') { next(); emit(T_FALSE); }
	emit(T_ERROR);
}

static token_t scan_atom(lexer_t *l) {
	if (curr() != ':') { errmit("expected ':'"); }

	next();
	if (!is_atom_start(curr())) { errmit("invalid atom"); }

	int start = l->pos, len = 1;

	next();
	while (is_atom_body(curr())) {
		len++;
		next();
	}

	lexer_copy_str(l, start, len);

	emit(T_ATOM);
}

static token_t scan_ident(lexer_t *l) {
	if (!is_ident_start(curr())) { errmit("invalid ident"); }

	int start = l->pos, len = 1;

	next();
	while (is_ident_body(curr())) {
		len++;
		next();
	}

	lexer_copy_str(l, start, len);

	emit(T_IDENT);
}

//
//

token_t lexer_next(lexer_t *l) {
	l->curr_int		= 0;
	l->curr_str[0]	= '\0';

	while (is_whitespace(curr())) {
		if (curr() == '\n') l->line++;
		l->pos++;
	}

	switch (curr()) {
		case 0:		{ emit(T_EOF); }
		case '(':	{ next(); emit(T_L_PAREN); }
		case ')':	{ next(); emit(T_R_PAREN); }
		case '"':	return scan_string(l);
		case '#':	return scan_boolean(l);
		case ':':	return scan_atom(l);
		default:
		{
			if (curr() == '-' || curr() == '+') {
				next();
				if (curr() >= '0' && curr() <= '9') {
					back();
					return scan_number(l);
				} else {
					back();
					return scan_ident(l);
				}
			} else if (curr() >= '0' && curr() <= '9') {
				return scan_number(l);
			} else if (is_ident_start(curr())) {
				return scan_ident(l);
			} else {
				errmit("unexpected character");
			}
		}
	}
}

token_t lexer_current_token(lexer_t *l) {
	return l->token;
}

INT lexer_current_int(lexer_t *l) {
	return l->curr_int;
}

const char* lexer_current_str(lexer_t *l) {
	return l->curr_str;
}
