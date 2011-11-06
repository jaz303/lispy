#include "lispy/eval.h"

#define ENSURE_ARITY(lst, arity, error) \
	if (list_len(lst) != arity) { \
		return kError; \
	}
	
#define ENSURE_MIN_ARITY(lst, min_arity, error) \
	if (list_len(lst) < min_arity) { \
		return kError; \
	}
	
#define ENSURE_MAX_ARITY(lst, max_arity, error) \
	if (list_len(lst) > max_arity) { \
		return kError; \
	}

VALUE eval_list(state_t *state, VALUE v) {
	ENSURE_MIN_ARITY(v, 1, "can't eval an empty list");
	EVAL(state, list_get(v, 0));
	// step 1 - check for head being ident representing a special form; dispatch if so
	// step 2 - check for head being ident in symbol table; dispatch if so
	// step 3 - explode
	return kError;
}

VALUE eval_ident(state_t *state, VALUE v) {
	// look up list ix 0 in symbol table
	// eval associated function
	return kError;
}

VALUE eval_do(state_t *state, VALUE v) {
	list_t *list = AS_LIST(v);
	int i;
	VALUE out;
	for (i = 1; i < list_len(list); i++) {
		out = EVAL(state, list_get(list, i));
	}
	return out;
}

VALUE eval_if(state_t *state, VALUE v) {
	list_t *list = AS_LIST(v);
	if (VALUE_IS_TRUTHY(EVAL(state, list_get(list, 1)))) {
		return EVAL(state, list_get(list, 2));
	} else {
		return EVAL(state, list_get(list, 3));
	}
}

VALUE eval_println(state_t *state, VALUE v) {
	return kError;
}

VALUE eval_quote(state_t *state, VALUE v) {
	ENSURE_ARITY(v, 2, "'quote' requires 1 argument");
	return list_get(v, 1);
}

VALUE eval_set(state_t *state, VALUE v) {
	ENSURE_ARITY(v, 3, "'set' form requires 2 arguments");
	return kError;
}

