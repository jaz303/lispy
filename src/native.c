#include "lispy/native.h"
#include "lispy/eval.h"

#define READ_INT(ix) \
    VALUE v##ix = EVAL(list_get(list, ix)); \
    if (!VALUE_IS_INT(v##ix)) return kError;

LISPY_NATIVE_FN(native_plus) {
    switch (list_len(list)) {
        case 1:
            return MK_INTVAL(0);
        case 2:
        {
            READ_INT(1);
            return MK_INTVAL(INTVAL(v1));
        }
        case 3:
        {
            READ_INT(1);
            READ_INT(2);
            return MK_INTVAL(INTVAL(v1) + INTVAL(v2));
        }
        case 4:
        {
            READ_INT(1);
            READ_INT(2);
            READ_INT(3);
            return MK_INTVAL(INTVAL(v1) + INTVAL(v2) + INTVAL(v3));
        }
        default:
        {
            INT sum = 0;
            int i;
            for (i = 1; i < list_len(list); i++) {
                VALUE v = EVAL(list_get(list, i));
                if (VALUE_IS_INT(v)) {
                    sum += INTVAL(v);
                } else {
                    return kError;
                }
            }
            return MK_INTVAL(sum);
        }    
    }
}
