#include "lispy/gc.h"
#include "lispy/binding.h"

#ifdef _LP64
	typedef uint64_t INTPTR;
#else
	typedef uint32_t INTPTR;
#endif

#define GC_NEXT(obj)        ((gc_object_t*)(((INTPTR)obj->next) & ~1))
#define GC_IS_MARKED(obj)   (((INTPTR)obj->next) & 1)
#define GC_MARK(obj)        (obj->next = (gc_object_t*)(((INTPTR)obj->next)|1))
#define GC_CLEAR(obj)       (obj->next = (gc_object_t*)(((INTPTR)obj->next)&~1))

static void gc_mark(gc_mgr_t *mgr);
static void gc_sweep(gc_mgr_t *mgr);
static void gc_walk(gc_object_t *obj);

void *gc_alloc(gc_mgr_t *mgr, size_t sz) {
    gc_object_t *obj = malloc(sizeof(gc_object_t) + sz);
    if (!obj) return NULL;
    obj->next = mgr->head;
    mgr->head = obj;
    return &obj->data;
}

void gc_run(gc_mgr_t *mgr) {
    gc_mark(mgr);
    gc_sweep(mgr);
}

void gc_mark(gc_mgr_t *mgr) {
    gc_walk(mgr->root);
}

void gc_sweep(gc_mgr_t *mgr) {
    gc_object_t *curr = mgr->head, *prev = NULL, *next = NULL;
    while (curr) {
        next = GC_NEXT(curr);
        if (!GC_IS_MARKED(curr)) {
            free(curr);
            if (prev) {
                prev->next = next;
            } else {
                mgr->head = next;
            }
        } else {
            GC_CLEAR(curr);
            prev = curr;
        }
        curr = next;
    }
}

void gc_walk(gc_object_t *obj) {
    GC_MARK(obj);
    // if obj is list, walk list contents
    // if obj is closure, walk its environment
}

list_t* gc_alloc_list(gc_mgr_t *mgr, size_t sz) {
    list_t *list = gc_alloc(mgr, sizeof(list_t) + sizeof(VALUE) * sz);
    if (list) {
        list->obj.type = TYPE_LIST;
        list->length = sz;
    }
    return list;
}

string_t* gc_alloc_string(gc_mgr_t *mgr, const char *str) {
    size_t len = strlen(str);
    string_t *string = gc_alloc(mgr, sizeof(string_t) + len + 1);
    if (string) {
        string->obj.type = TYPE_STRING;
        string->length = len;
        strcpy(string->string, str);
    }
    return string;
}

float_t* gc_alloc_float(gc_mgr_t *mgr, float val) {
    float_t *flt = gc_alloc(mgr, sizeof(float_t));
    if (flt) {
        flt->obj.type = TYPE_FLOAT;
        flt->value = val;
    }
    return flt;
}

binding_t* gc_alloc_binding(gc_mgr_t *mgr, binding_t *parent) {
    binding_t *binding = gc_alloc(mgr, sizeof(binding_t));
    if (!binding_init(binding, parent)) {
        // TODO: cleanup
        return NULL;
    }
    return binding;
}

native_fn_t* gc_alloc_native_fn(gc_mgr_t *mgr, size_t arity, native_fn *native) {
    native_fn_t *fn = gc_alloc(mgr, sizeof(native_fn_t));
    if (fn) {
        fn->obj.type = TYPE_NATIVE_FN;
        fn->arity = arity;
        fn->fn = native;
    }
    return fn;
}