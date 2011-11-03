#include "lispy/parser.h"
#include "lispy/value.h"
#include "lispy/global.h"

#include <stdlib.h>
#include <stdio.h>

#define curr()				(lexer_current_token(p->lexer))
#define accept()			(lexer_next(p->lexer))
#define accept_tok(tok)		((curr() == tok) ? (accept(),1) : 0)

#define PARSE_ERROR(msg)	p->error = (msg)

static int parse_list(parser_t *, list_t **list);
static int parse_value(parser_t *, VALUE *value);

void parser_init(parser_t *p) {
    p->error = NULL;
}

list_t *parser_parse(parser_t *p) {
	lexer_next(p->lexer);
	list_t *ast;
	if (parse_list(p, &ast)) {
		if (accept_tok(T_EOF)) {
			return ast;
		} else {
			PARSE_ERROR("expecting EOF");
			obj_dealloc(ast);
		}
	}
	return NULL;
}

struct tmp_list_node;
struct tmp_list_node {
	VALUE value;
	struct tmp_list_node *next;
};

static void free_tmp_list(struct tmp_list_node *head) {
	while (head) {
		struct tmp_list_node *next = head->next;
		OBJ_SAFE_DEALLOC(head->value);
		free(head);
		head = next;
	}
}

int parse_list(parser_t *p, list_t **list) {
	size_t length = 0;
	struct tmp_list_node *head = NULL, *tail = NULL;
	
	if (!accept_tok(T_L_PAREN)) {
		PARSE_ERROR("expecting '('");
		goto error;
	}
	
	while (curr() != T_R_PAREN) {
		VALUE parsed;
		if (parse_value(p, &parsed)) {
			struct tmp_list_node *tmp = malloc(sizeof(struct tmp_list_node));
			if (!tmp) {
				PARSE_ERROR("failed to allocate temporary list node");
				OBJ_SAFE_DEALLOC(parsed);
				goto error;
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
			goto error;
		}
	}
	
	if (!accept_tok(T_R_PAREN)) {
		PARSE_ERROR("expecting ')'");
		goto error;
	}

	*list = list_create(NULL, length);
	if (!*list) {
		PARSE_ERROR("failed to allocate list object");
		goto error;
	}
	
	size_t ix = 0;
	struct tmp_list_node *curr = head;
	while (curr) {
		(*list)->values[ix++] = curr->value;
		curr = curr->next;
	}
	
	free_tmp_list(head);
	return 1;
	
error:
	
	free_tmp_list(head);
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

	PARSE_ERROR("unexpected token in input");
	return 0;
}
