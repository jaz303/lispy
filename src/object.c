#include "lispy/object.h"

#include <string.h>

// assumes 32 bit alignment :/
#define ROUND(val) ((val + 3) & ~0x03)
#define OFFSET(obj, bytes) (((char*)obj) + bytes)

//
// obj def structs for built-in types

static void list_dealloc(void*);
obj_def_t __obj_type_list = {
	list_dealloc
};

obj_def_t __obj_type_string = {
	NULL
};

obj_def_t __obj_type_float = {
	NULL
};

//
// obj

void obj_dealloc(void *obj) {
	obj_t *object = (obj_t*)obj;
	if (object->is_a->dealloc) {
		object->is_a->dealloc(object);
	}
	ALLOCATOR_FREE(object->allocator, object);
}

//
// list

list_t*	list_create(allocator_t *allocator, size_t length) {
	size_t obj_size  = ROUND(sizeof(list_t));
	size_t data_size = sizeof(VALUE) * length;
	
	list_t *list = ALLOCATOR_ALLOC(allocator, obj_size + data_size);
	if (list) {
		OBJ_SETUP(list, TYPE_LIST, allocator);
		list->length = length;
		list->values = (VALUE*) OFFSET(list, obj_size);
	}
	
	return list;
}

static void list_dealloc(void *list) {
	list_t *self = (list_t*) list;
	int i;
	for (i = 0; i < self->length; i++)
		OBJ_SAFE_DEALLOC(self->values[i]);
}

//
// string

string_t* string_create(allocator_t *allocator, char *str) {
	string_t *string = ALLOCATOR_ALLOC(allocator, sizeof(string_t));
	if (string) {
		OBJ_SETUP(string, TYPE_STRING, allocator);
		string->length = strlen(str);
		string->string = str;
	}
	return string;
}

string_t* string_create_copy(allocator_t *allocator, const char *str) {
	size_t obj_size = ROUND(sizeof(string_t));
	size_t str_size = sizeof(char) * (strlen(str) + 1);
	
	string_t *string = ALLOCATOR_ALLOC(allocator, obj_size + str_size);
	if (string) {
		OBJ_SETUP(string, TYPE_STRING, allocator);
		string->length = strlen(str);
		string->string = (char*) OFFSET(string, obj_size);
		strcpy(string->string, str);
	}
	
	return string;
}

//
// float

float_t* float_create(allocator_t *allocator, float val) {
	float_t *fl = ALLOCATOR_ALLOC(allocator, sizeof(float_t));
	if (fl) {
		OBJ_SETUP(fl, TYPE_FLOAT, allocator);
		fl->value = val;
	}
	return fl;
}

