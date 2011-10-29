#ifndef OBJECT_H
#define OBJECT_H

/*
 * Objects
 */

#include <stdlib.h>

#include "lispy/global.h"

#define OBJ_TYPE_SEXP_AST		1
#define OBJ_TYPE_SEXP			2
#define OBJ_TYPE_STRING		3
#define OBJ_TYPE_FLOAT		4

#define OBJ_TYPE_IS(o, t)		(((obj_t*)o)->type == t)
#define OBJ_IS_SEXP_AST(o)	(OBJ_TYPE_IS(o, OBJ_TYPE_SEXP_AST))
#define OBJ_IS_SEXP(o)		(OBJ_TYPE_IS(o, OBJ_TYPE_SEXP))
#define OBJ_IS_STRING(o)		(OBJ_TYPE_IS(o, OBJ_TYPE_STRING))
#define OBJ_IS_FLOAT(o)		(OBJ_TYPE_IS(o, OBJ_TYPE_FLOAT))

//
// base object

typedef struct obj {
	int			type;
} obj_t;

//
// intermediate linked-list representation of sexp

typedef struct obj_sexp_ast_node obj_sexp_ast_node_t;
struct obj_sexp_ast_node {
	VALUE					value;
	obj_sexp_ast_node_t		*next;
};

typedef struct obj_sexp_ast {
	obj_t					obj;
	size_t					length;
	obj_sexp_ast_node_t		*head;
	obj_sexp_ast_node_t		*tail;
} obj_sexp_ast_t;

//
// compacted representation of sexp

typedef struct obj_sexp {
	obj_t		obj;
	size_t		length;
	VALUE		*values;
} obj_sexp_t;

//
// string

typedef struct string {
	obj_t		obj;
	size_t		length;
	char			*string;
} string_t;

//
// float

typedef struct float_obj {
	obj_t		obj;
	float		value;
} float_obj_t;

//
//

string_t*		obj_mk_string(char *str);
string_t*		obj_mk_string_copy(char *str);

float_obj_t*		obj_mk_float(float val);

#endif // OBJECT_H
