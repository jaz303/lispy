#include "lispy/object.h"

#include <string.h>

list_t* list_create(void *allocator, size_t length) {
	list_t *list = malloc(sizeof(list_t));
	if (list) {
		list->obj.type = OBJ_TYPE_LIST;
		list->length = length;
		list->values = malloc(sizeof(VALUE) * length);
		if (!list->values) {
			free(list);
			list = NULL;
		}
	}
	return list;
}

string_t* string_create(void *allocator, char *str) {
	string_t *string = malloc(sizeof(string_t));
	string->obj.type = OBJ_TYPE_STRING;
	string->length = strlen(str);
	string->string = str;
	return string;
}

string_t* string_create_copy(void *allocator, const char *str) {
	char *str_copy = malloc(sizeof(char) * (strlen(str) + 1));
	if (!str_copy) return NULL;
	strcpy(str_copy, str);
	string_t *out = string_create(allocator, str_copy);
	if (!out) free(str_copy);
	return out;
}

float_t* float_create(void *allocator, float val) {
	float_t *fl = malloc(sizeof(float_t));
	if (fl) {
		fl->obj.type = OBJ_TYPE_FLOAT;
		fl->value = val;
	}
	return fl;
}
