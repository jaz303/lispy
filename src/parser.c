#include "lispy/parser.h"
#include "lispy/value.h"
#include "lispy/global.h"

#include <stdlib.h>
#include <stdio.h>

#define curr()				(lexer_current_token(p->lexer))
#define accept()				(lexer_next(p->lexer))
#define accept2(tok, msg)		if (curr() != tok) { p->error = msg; goto error; } else { accept(); }

static int parse_list(parser_t *, list_t **list);
static int parse_value(parser_t *, VALUE *value);

void parser_init(parser_t *p) {
    p->error = NULL;
}

list_t *parser_parse(parser_t *p) {
	lexer_next(p->lexer);
	list_t *ast;
	if (parse_list(p, &ast)) {
		if (curr() != T_EOF) {
			p->error = "expecting EOF";
			// TODO: free list
		} else {
			accept();
			return ast;
		}
	}
	return NULL;
}

struct tmp_list_node;
struct tmp_list_node {
	VALUE value;
	struct tmp_list_node *next;
};

int parse_list(parser_t *p, list_t **list) {
	size_t length = 0;
	struct tmp_list_node *head = NULL, *tail = NULL;

	accept2(T_L_PAREN, "expecting '('");

	while (curr() != T_R_PAREN) {
		VALUE parsed;
		if (parse_value(p, &parsed)) {
			struct tmp_list_node *tmp = malloc(sizeof(struct tmp_list_node));
			if (!tmp) {
				// cleanup
			} else {
				length++;
				tmp->next = NULL;
				tmp->value = parsed;
				if (head == NULL) {
					head = tmp;
				} else {
					tail->next = tmp;
				}
				tail = tmp;
			}
		} else {
			// cleanup
			return 0;
		}
	}

	accept2(T_R_PAREN, "expecting ')'");

	*list = list_create(NULL, length);
	if (!*list) goto error;

	{
		struct tmp_list_node *curr = head, *tmp;
		size_t ix = 0;
		while (curr) {
			(*list)->values[ix++] = curr->value;
			tmp = curr->next;
			free(curr);
			curr = tmp;
		}
	}

	return 1;

error:
	{
		struct tmp_list_node *curr = head, *tmp;
		while (curr) {
			tmp = curr->next;
			free(curr);
			curr = tmp;
		}
	}

	return 0;

}

int parse_value(parser_t *p, VALUE *value) {
	switch (curr()) {
		case T_INT:
		{
			*value = MK_INTVAL(lexer_current_int(p->lexer));
			accept();
			return 1;
		}
		case T_L_PAREN:
		{
			return parse_list(p, (list_t**)value);
		}
		case T_TRUE:
		{
			*value = kTrue;
			accept();
			return 1;
		}
		case T_FALSE:
		{
			*value = kFalse;
			accept();
			return 1;
		}
		case T_ATOM:
		{
			*value = MK_ATOM(intern_table_put(p->intern, lexer_current_str(p->lexer)));
			accept();
			return 1;
		}
		case T_IDENT:
		{
			*value = MK_IDENT(intern_table_put(p->intern, lexer_current_str(p->lexer)));
			accept();
			return 1;
		}
		case T_STRING:
		{
			*value = string_create_copy(NULL, lexer_current_str(p->lexer));
			accept();
			return 1;
		}
	}

	p->error = "Unexpected token in input";
	return 0;
}
