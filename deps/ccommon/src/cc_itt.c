
#include <cc_itt.h>
#include <cc_mm.h>
#include <cc_log.h>
#include <cc_debug.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define ITT_MODULE_NAME "ccommon::itt"
#define ITT_DOMAIN_NAME "cc_itt"

static bool itt_init = false;

#ifdef CC_ITT
__itt_heap_function cc_itt_malloc;
__itt_heap_function cc_itt_free;
__itt_heap_function cc_itt_realloc;

static void
_cc_initialize(void)
{
    cc_itt_malloc = __itt_heap_function_create("allocate", ITT_DOMAIN_NAME);
    cc_itt_free = __itt_heap_function_create("free", ITT_DOMAIN_NAME);
    cc_itt_realloc =  __itt_heap_function_create("reallocate", ITT_DOMAIN_NAME);
}

#else
#define _cc_initialize(...)

#endif

void
itt_setup(void)
{
    log_stderr("set up the %s module", ITT_MODULE_NAME);

    if (itt_init) {
        log_stderr("%s has already been setup, overwrite", ITT_MODULE_NAME);
    }

    _cc_initialize();

    itt_init = true;
}

void
itt_teardown(void)
{
    log_stderr("tear down the %s module", ITT_MODULE_NAME);

    if (!itt_init) {
        log_stderr("%s has never been setup", ITT_MODULE_NAME);
    }

    itt_init = false;
}
