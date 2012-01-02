#ifndef GC_H
#define GC_H

#include "lispy/lispy.h"

void*           gc_alloc(gc_mgr_t *mgr, size_t sz);
void            gc_run(gc_mgr_t *mgr);

list_t*         gc_alloc_list(gc_mgr_t *mgr, size_t sz);
string_t*       gc_alloc_string(gc_mgr_t *mgr, const char *str);
float_t*        gc_alloc_float(gc_mgr_t *mgr, float val);
binding_t*      gc_alloc_binding(gc_mgr_t *mgr, binding_t *parent);
native_fn_t*    gc_alloc_native_fn(gc_mgr_t *mgr, size_t arity, native_fn *fn);

#endif