#ifndef LISPY_H
#define LISPY_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef _LP64
	typedef void*       VALUE;
	typedef int64_t     INT;
	typedef uint64_t    INTERN;
#else
	typedef void*       VALUE;
	typedef int32_t     INT;
	typedef uint32_t    INTERN;
#endif

typedef enum token {
	T_ERROR			= -1,
	T_EOF			=  0,
	T_L_PAREN		=  1,
	T_R_PAREN		=  2,
	T_INT			=  3,
	T_TRUE			=  4,
	T_FALSE			=  5,
	T_STRING		=  6,
	T_ATOM			=  7,
	T_IDENT			=  8,
	T_NIL           =  9,
} token_t;

/* Forward Declarations */

typedef struct intern_table intern_table_t;
typedef struct env env_t;

/* values */

#include "lispy/value.h"

/* objects */

#define TYPE_LIST           1
#define TYPE_STRING         2
#define TYPE_FLOAT          3
#define TYPE_BINDING        4
#define TYPE_NATIVE_FN      5
#define TYPE_LAMBDA         6

#define IS_OBJECT(v)		(VALUE_IS_PTR(v))
#define IS_A(v, t)          (((obj_t*)v)->type == t)

#define IS_LIST(v)			(IS_OBJECT(v) && IS_A(v, TYPE_LIST))
#define IS_STRING(v)		(IS_OBJECT(v) && IS_A(v, TYPE_STRING))
#define IS_FLOAT(v)			(IS_OBJECT(v) && IS_A(v, TYPE_FLOAT))
#define IS_BINDING(v)       (IS_OBJECT(v) && IS_A(v, TYPE_BINDING))
#define IS_NATIVE_FN(v)     (IS_OBJECT(v) && IS_A(v, TYPE_NATIVE_FN))
#define IS_LAMBDA(v)        (IS_OBJECT(v) && IS_A(v, TYPE_LAMBDA))

#define AS_LIST(v)			((list_t*)v)
#define AS_STRING(v)		((string_t*)v)
#define AS_FLOAT(v)			((float_t*)v)
#define AS_BINDING(v)       ((binding_t*)v)
#define AS_NATIVE_FN(v)     ((native_fn_t*)v)
#define AS_LAMBDA(v)        ((lambda_t*)v)

typedef struct obj {
    int                 type;
} obj_t;

typedef struct {
	obj_t		obj;							/* object header */
	size_t		length;							/* number of values in list */
//	VALUE		(*form)(state_t*, VALUE);		/* evaluation function for this list */
    VALUE       values[0];                      /* array of list values */
} list_t;

typedef struct {
	obj_t		obj;
	size_t		length;
	char		string[0];
} string_t;

typedef struct {
	obj_t		obj;
	float		value;
} float_t;

#include "jazlib/gen_hash_reset.h"
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STORAGE(__binding_table, INTERN, VALUE);

typedef struct binding binding_t;
struct binding {
    obj_t               obj;
    binding_t           *parent;
    __binding_table_t   table;
};

typedef VALUE (native_fn)(env_t*, binding_t*, list_t*);
#define LISPY_NATIVE_FN(name) VALUE name(env_t *env, binding_t *binding, list_t *list)

typedef struct {
    obj_t       obj;
    size_t      arity;
    native_fn   *fn;
} native_fn_t;

typedef struct {
    obj_t       obj;
    binding_t   *binding;
    size_t      arity;
    list_t      *args;
    list_t      *defn;
} lambda_t;

#define list_len(list)              (AS_LIST(list)->length)
#define list_get(list, ix)          (AS_LIST(list)->values[ix])
#define list_set(list, ix, val)     (AS_LIST(list)->values[ix]=(VALUE)val)
       
#define string_len(str)             (AS_STRING(str)->length)
#define string_chars(str)           (AS_STRING(str)->string)
       
#define float_value(fl)             (AS_FLOAT(fl)->value)

/* GC primitives */

typedef struct gc_object gc_object_t;

struct gc_object {
    gc_object_t     *next;
    char            data[0];
};

typedef struct gc_mgr {
    gc_object_t     *root;
    gc_object_t     *head;
} gc_mgr_t;

/* Interning */

#include "jazlib/gen_hash_reset.h"
#include "jazlib/gen_hash.h"
GEN_HASH_DECLARE_STORAGE(__intern_s2i, const char *, INTERN);

struct intern_table {
    size_t          size;                   /* number of strings in table */
    size_t          strings_sz;             /* size of strings array */
    char            **strings;              /* strings, indexed by intern val */
    __intern_s2i_t  string_to_intern;       /* hash string => intern */
};

/* Execution environment */

struct env {
    intern_table_t      intern;
    gc_mgr_t            gc;
    binding_t           binding;
};

/* Lexer & Parser */

typedef struct lexer {
	const char		*text;              /* text we're lexing */
	int				pos;                /* current pos in text */
	INT				curr_int;           /* int value of current token */
	char			*curr_str;          /* null-terminated string value of current token */
	size_t			curr_str_sz;        /* current size of string buffer */
	token_t			token;              /* current token */
	int				line;               /* current line */
	const char		*error;             /* error */
} lexer_t;

typedef struct parser {
	lexer_t			*lexer;
    env_t           *env;
	const char		*error;
} parser_t;

#endif