#include "lispy/parser.h"

#include <stdlib.h>

#define PARSE_ERROR(msg) p->error = msg

static obj_sexp_ast_t*	parse_sexp(parser_t *);
static VALUE				parse_value(parser_t *);
static obj_sexp_t*		parser_compact(parser_t *, obj_sexp_ast_t *);

void	 parser_init(parser_t *parser, const char *source) {
	parser->source = source;
}

void *parser_ast_alloc(parser_t *p, size_t sz) {
	return malloc(sz);
}

void parser_ast_free(parser_t *p, void *thing) {
	free(thing);
}

void* parser_sexp_alloc(parser_t *p, size_t sz) {
	return malloc(sz);
}

void parser_sexp_free(parser_t *p, void *thing) {
	free(thing);
}

obj_sexp_t* parser_parse(parser_t *p) {
	obj_sexp_ast_t *ast = parse_sexp(p);
	return parsed ? parser_compact(p, ast) : NULL;
}

obj_sexp_ast_t* parse_sexp(parser_t *p) {
	obj_sexp_ast_t *sexp = parser_ast_alloc(p, sizeof(obj_sexp_ast_t));
	if (sexp == NULL) {
		PARSE_ERROR("alloc");
		return NULL;
	}

	sexp->length	= 0;
	sexp->head	= NULL;
	sexp->tail	= NULL;

	accept(T_L_PAREN);

	while (curr() != T_R_PAREN) {
		VALUE parsed = parse_value(p);
		if (p->error) break;

		obj_sexp_ast_node_t *node = parser_ast_alloc(p, sizeof(obj_sexp_ast_node_t));
		if (node == NULL) {

		}

		node->next = NULL;
		node->value = parsed;

		if (sexp->head == NULL) {
			sexp->head = node;
		} else {
			sexp->tail->next = node;
		}

		sexp->tail = node;
		sexp->length++;
	}

	accept(T_R_PAREN);

	return NULL;
}

VALUE parse_value(parser_t *p) {
	VALUE out = 0;
	switch (curr()) {
		case T_INTEGER:
		{
			out = MK_INTVAL(0); // TODO: get value
		}
		case T_L_PAREN:
		{
			out = parse_sexp(p);
		}
		case T_TRUE:
		{
			out = kTrue;
		}
		case T_FALSE:
		{
			out = kFalse;
		}
		case T_ATOM:
		{
			out = MK_ATOM(0);
		}
		case T_IDENT:
		{
			out = MK_IDENT(0);
		}
		case T_STRING:
		{
			out = obj_mk_string_copy("");
		}
		default:
		{

		}
	}
	return out;
}

obj_sexp_t *parser_compact(parser_t *p, obj_sexp_ast_t *ast) {

	obj_sexp_t *sexp = parser_sexp_alloc(p, sizeof(obj_sexp_t));
	if (sexp == NULL) {
		goto error;
	} else {
		sexp->obj.type = OBJ_TYPE_SEXP;
		sexp->length = ast->length;
		sexp->values = NULL;
	}

	if (ast->length > 0) {
		sexp->values = parser_sexp_alloc(p, sizeof(VALUE) * ast->length);
		if (sexp->values == NULL) goto error;
	} else {
		sexp->values = NULL;
	}

	obj_sexp_ast_node_t *curr = ast->head;
	int ix = 0;

	while (curr) {
		if (OBJ_IS_SEXP_AST(curr->value)) {
			sexp->values[ix] = parser_compact(p, (obj_sexp_ast_t *)curr->value);
		} else {
			sexp->values[ix] = curr->value;
		}
		curr = curr->next;
		ix++;
	}

	return sexp;

error:

	if (sexp) {
		if (sexp->values) parser_sexp_free(p, sexp->values);
		parser_sexp_free(p, sexp);
	}

	return NULL;

}
