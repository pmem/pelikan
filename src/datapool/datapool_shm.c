/*
 * Anonymous shared memory backed datapool.
 * Loses all its contents after closing.
 */
#include "datapool.h"

#include <cc_debug.h>
#include <cc_mm.h>

#define DATAPOOL_MODULE_NAME "datapool::shm"

static bool datapool_init = false;

static char* datapool_path = DATAPOOL_PATH;

datapool_medium_e
datapool_get_medium(void)
{
    return DATAPOOL_MEDIUM_SHM;
}

void
datapool_setup(datapool_options_st *options)
{
    log_info("set up the %s module", DATAPOOL_MODULE_NAME);

    if (datapool_init) {
        log_warn("datapool has already been setup, re-creating");
        datapool_teardown();
    }

    if (options != NULL) {
        datapool_path = option_str(&options->datapool_path);
    }
    datapool_init = true;
}

void
datapool_teardown(void)
{
    log_info("tear down the %s module", DATAPOOL_MODULE_NAME);

    if (!datapool_init) {
        log_warn("%s has never been setup", DATAPOOL_MODULE_NAME);
    }

    datapool_init = false;
}

struct datapool *
datapool_open(size_t size, int *fresh)
{
    if (datapool_path != NULL) {
        log_warn("attempted to open a file-based data pool without"
            "pmem features enabled");
        return NULL;
    }

    if (fresh) {
        *fresh = 1;
    }

    return cc_zalloc(size);
}

void
datapool_close(struct datapool *pool)
{
    cc_free(pool);
}

void *
datapool_addr(struct datapool *pool)
{
    return pool;
}

size_t
datapool_size(struct datapool *pool)
{
    return cc_alloc_usable_size(pool);
}

/*
 * NOTE: Abstraction in datapool required defining functions below
 *       datapool_get_user_data is currently used only in in pmem implementation
 *       datapool_set_user_data is called during teardown e.g. slab
 */
void
datapool_set_user_data(const struct datapool *pool, const void *user_data, size_t user_size)
{

}

void
datapool_get_user_data(const struct datapool *pool, void *user_data, size_t user_size)
{
    NOT_REACHED();
}
