#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <cc_define.h>

#ifdef CC_ITT
#include "ittnotify.h"

#define ITT_DOMAIN_NAME "cc_itt"

#define DECLARE_ITT_MALLOC(_name)                                                   \
    __itt_heap_function _name = __itt_heap_function_create(#_name, ITT_DOMAIN_NAME)

#define DECLARE_ITT_FREE(_name)                                                     \
    __itt_heap_function _name = __itt_heap_function_create(#_name, ITT_DOMAIN_NAME)

#define DECLARE_ITT_REALLOC(_name)                                                  \
    __itt_heap_function _name = __itt_heap_function_create(#_name, ITT_DOMAIN_NAME)

#define cc_itt_alloc_begin(_itt_heap_f, _s)                                         \
    __itt_heap_allocate_begin(_itt_heap_f, (size_t)(_s), 0)

#define cc_itt_alloc_end(_itt_heap_f, _p, _s)                                       \
    __itt_heap_allocate_end(_itt_heap_f, &(_p), (size_t)(_s), 0)

#define cc_itt_zalloc_begin(_itt_heap_f, _s)                                        \
    __itt_heap_allocate_begin(_itt_heap_f, (size_t)(_s), 1)

#define cc_itt_zalloc_end(_itt_heap_f, _p, _s)                                      \
    __itt_heap_allocate_end(_itt_heap_f, &(_p), (size_t)(_s), 1)

#define cc_itt_free_begin(_itt_heap_f, _p)                                          \
    __itt_heap_free_begin(_itt_heap_f, _p)

#define cc_itt_free_end(_itt_heap_f, _p)                                            \
    __itt_heap_free_end(_itt_heap_f, _p)

#define cc_itt_realloc_begin(_itt_heap_f, _p, _s)                                   \
    __itt_heap_reallocate_begin(_itt_heap_f, _p, (size_t)(_s), 0)

#define cc_itt_realloc_end(_itt_heap_f, _p, _np, _s)                                \
    __itt_heap_reallocate_end(_itt_heap_f, _p, &(_np), (size_t)(_s), 0)

#define cc_itt_heap_internal_access_begin()                                         \
    __itt_heap_internal_access_begin()

#define cc_itt_heap_internal_access_end()                                           \
    __itt_heap_internal_access_end()

#else
#define DECLARE_ITT_MALLOC(_name)
#define DECLARE_ITT_FREE(_name)
#define DECLARE_ITT_REALLOC(_name)
#define cc_itt_alloc_begin(_itt_heap_f, _s)
#define cc_itt_alloc_end(_itt_heap_f, _p, _s)
#define cc_itt_zalloc_begin(_itt_heap_f, _s)
#define cc_itt_zalloc_end(_itt_heap_f, _p, _s)
#define cc_itt_free_begin(_itt_heap_f, _p)
#define cc_itt_free_end(_itt_heap_f, _p)
#define cc_itt_realloc_begin(_itt_heap_f, _p, _s)
#define cc_itt_realloc_end(_itt_heap_f, _p, _np, _s)
#define cc_itt_heap_internal_access_begin()
#define cc_itt_heap_internal_access_end()
#endif /* CC_ITT */

#ifdef __cplusplus
}
#endif
