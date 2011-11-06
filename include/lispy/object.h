#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include "lispy/global.h"
#include "lispy/value.h"

//
// type macros

#define TYPE_LIST 			(&__obj_type_list)
#define TYPE_STRING			(&__obj_type_string)
#define TYPE_FLOAT			(&__obj_type_float)

#define IS_OBJECT(v)		(VALUE_IS_PTR(v))
#define IS_A(obj, type) 	(((obj_t*)obj)->is_a == type)
#define IS_LIST(v)			(IS_OBJECT(v) && IS_A(v, TYPE_LIST))
#define IS_STRING(v)		(IS_OBJECT(v) && IS_A(v, TYPE_STRING))
#define IS_FLOAT(v)			(IS_OBJECT(v) && IS_A(v, TYPE_FLOAT))

//
// allocation macros

#define ALLOCATOR_ALLOC(a, sz)			(a == NULL ? (malloc(sz)) : (a->alloc(a, sz)))
#define ALLOCATOR_REALLOC(a, ptr, sz)	(a == NULL ? (realloc(ptr, sz)) : (a->realloc(a, ptr, sz)))
#define ALLOCATOR_FREE(a, ptr)			(a == NULL ? (free(ptr)) : (a->free(a, ptr)))

#define OBJ_SAFE_DEALLOC(obj)			(IS_OBJECT(obj) ? (obj_dealloc(obj),1) : 0)

#define OBJ_SETUP(obj, type, allocator) \
	((obj_t*)obj)->is_a = type; \
	((obj_t*)obj)->allocator = allocator

//
// allocator

typedef struct allocator allocator_t;

struct allocator {
	void*	(*alloc)(allocator_t*, size_t);
	void*	(*realloc)(allocator_t*, void*, size_t);
	void	(*free)(allocator_t*, void*);
	void*	userdata;
};

//
// obj def

typedef struct obj_def {
	void (*dealloc)(void*);	
} obj_def_t;

extern obj_def_t __obj_type_list;
extern obj_def_t __obj_type_string;
extern obj_def_t __obj_type_float;

//
// obj

typedef struct obj {
	obj_def_t*			is_a;
	allocator_t*		allocator;
} obj_t;

void obj_dealloc(void *obj);

//
// list

typedef struct list {
	obj_t		obj;							/* object header */
	size_t		length;							/* number of values in list */
	VALUE		*values;						/* array of list values */
	VALUE		(*form)(state_t*, VALUE);		/* evaluation function for this list */
		
} list_t;

list_t*	list_create(allocator_t *allocator, size_t length);
#define list_len(list) (((list_t*)list)->length)
#define list_get(list, ix) (((list_t*)list)->values[ix])
#define list_set(list, ix, val) (((list_t*)list)->values[ix]=(VALUE)val)

//
// string

typedef struct string {
	obj_t		obj;
	size_t		length;
	char		*string;
} string_t;

string_t*	string_create(allocator_t *allocator, char *str);
string_t*	string_create_copy(allocator_t *allocator, const char *str);
#define		string_len(string) (((string_t*)string)->length)

//
// float

typedef struct float_obj {
	obj_t		obj;
	float		value;
} float_t;

float_t* float_create(allocator_t *allocator, float val);

//
// 

#endif
