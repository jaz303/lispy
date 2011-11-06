#ifndef EVAL_H
#define EVAL_H

// http://norvig.com/lispy.html

#include "lispy/object.h"
#include "lispy/state.h"

#define EVAL(state, val) (IS_LIST(val) ? AS_LIST(val)->form(state, val) : val)

// the most generic evaluator
// this is selected when no other, more specific, form matches the given list
VALUE eval_list(state_t *state, VALUE v);

// any list not covered by any other special form, whose first element is an ident
VALUE eval_ident(state_t *state, VALUE v);

// (def key "value")
VALUE eval_def(state_t *state, VALUE v);

// (do (println "hello") (println "goodbye"))
VALUE eval_do(state_t *state, VALUE v);

// (get key)
VALUE eval_get(state_t *state, VALUE v);

// (if (condition) (if-true) (if-false))
VALUE eval_if(state_t *state, VALUE v);

// (println "foo")
VALUE eval_println(state_t *state, VALUE v);

// (quote (foo bar baz))
VALUE eval_quote(state_t *state, VALUE v);

// (set key "value")
VALUE eval_set(state_t *state, VALUE v);

#endif
