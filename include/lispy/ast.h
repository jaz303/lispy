#ifndef AST_H
#define AST_H

#include "lispy/value.h"

typedef struct sexp_ast_value sexp_ast_value_t;
struct sexp_ast_value {
	VALUE				value;
	sexp_ast_value_t		*next;
};

typedef struct sexp_ast {
	int					count;
	sexp_ast_value_t		*head;
	sexp_ast_value_t		*tail;
} sexp_ast_t;

// types:
// ident
// number
// sexp
// true|false
// string

// (+ "foo" "bar")



#endif // AST_H
