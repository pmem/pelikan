#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <cc_define.h>

void itt_setup(void);
void itt_teardown(void);

#ifdef CC_ITT
#include "ittnotify.h"
extern __itt_heap_function cc_itt_malloc;
extern __itt_heap_function cc_itt_free;
extern __itt_heap_function cc_itt_realloc;
#define cc_itt_alloc_begin(_s)                                             \
    __itt_heap_allocate_begin(cc_itt_malloc, (size_t)(_s), 0)

#define cc_itt_alloc_end(_p, _s)                                           \
    __itt_heap_allocate_end(cc_itt_malloc, &(_p), (size_t)(_s), 0)

#define cc_itt_zalloc_begin(_s)                                            \
    __itt_heap_allocate_begin(cc_itt_malloc, (size_t)(_s), 1)

#define cc_itt_zalloc_end(_p, _s)                                          \
    __itt_heap_allocate_end(cc_itt_malloc, &(_p), (size_t)(_s), 1)

#define cc_itt_free_begin(_p)                                              \
    __itt_heap_free_begin(cc_itt_free, _p)

#define cc_itt_free_end(_p)                                                \
    __itt_heap_free_end(cc_itt_free, _p)

#define cc_itt_realloc_begin(_p, _s)                                       \
    __itt_heap_reallocate_begin(cc_itt_realloc, _p, (size_t)(_s), 0)

#define cc_itt_realloc_end(_p, _np, _s)                                    \
    __itt_heap_reallocate_end(cc_itt_realloc, _p, &(_np), (size_t)(_s), 0)

#define cc_itt_heap_internal_access_begin()                                \
    __itt_heap_internal_access_begin()

#define cc_itt_heap_internal_access_end()                                  \
    __itt_heap_internal_access_end()

#else
#define cc_itt_alloc_begin(_s)
#define cc_itt_alloc_end(_p, _s)
#define cc_itt_zalloc_begin(_s)
#define cc_itt_zalloc_end(_p, _s)
#define cc_itt_free_begin(_p)
#define cc_itt_free_end(_p)
#define cc_itt_realloc_begin(_p, _s)
#define cc_itt_realloc_end(_p, _np, _s)
#define cc_itt_heap_internal_access_begin()
#define cc_itt_heap_internal_access_end()
#endif

#ifdef __cplusplus
}
#endif
