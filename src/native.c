#include "lispy/native.h"
#include "lispy/eval.h"

#define READ_INT(ix) \
    VALUE v##ix = EVAL(list_get(list, ix)); \
    if (!VALUE_IS_INT(v##ix)) return kError;

LISPY_NATIVE_FN(native_plus) {
    READ_INT(1);
    READ_INT(2);
    return MK_INTVAL(INTVAL(v1) + INTVAL(v2));
}
