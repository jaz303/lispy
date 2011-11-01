#ifndef OBJECT_H
#define OBJECT_H

/*
 * Objects
 */

#include <stdlib.h>

#include "lispy/global.h"

#define OBJ_TYPE_LIST			1
#define OBJ_TYPE_STRING		2
#define OBJ_TYPE_FLOAT		3

#define OBJ_TYPE_IS(o, t)		(((obj_t*)o)->type == t)
#define OBJ_IS_LIST(o)		(OBJ_TYPE_IS(o, OBJ_TYPE_LIST))
#define OBJ_IS_STRING(o)		(OBJ_TYPE_IS(o, OBJ_TYPE_STRING))
#define OBJ_IS_FLOAT(o)		(OBJ_TYPE_IS(o, OBJ_TYPE_FLOAT))

//
// base object

typedef struct obj {
	int			type;
} obj_t;

//
// compacted representation of sexp

typedef struct list {
	obj_t		obj;
	size_t		length;
	VALUE		*values;
} list_t;

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
} float_t;

//
//

list_t*			list_create(void *allocator, size_t length);

string_t*		string_create(void *allocator, char *str);
string_t*		string_create_copy(void *allocator, const char *str);

float_t*			float_create(void *allocator, float val);

#endif // OBJECT_H
