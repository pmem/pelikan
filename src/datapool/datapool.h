#pragma once

#include <cc_option.h>
#include <stddef.h>
#include <stdbool.h>

#define DATAPOOL_PATH NULL
#define DATAPOOL_NAME "datapool"
#define DATAPOOL_PREFAULT false

/*          name                type               default             description */
#define DATAPOOL_OPTION(ACTION)  \
    ACTION( datapool_path,      OPTION_TYPE_STR,   DATAPOOL_PATH,      "path to data pool"   )\
    ACTION( datapool_name,      OPTION_TYPE_STR,   DATAPOOL_NAME,      "datapool name"       )\
    ACTION( datapool_prefault,  OPTION_TYPE_BOOL,  DATAPOOL_PREFAULT,  "prefault datapool"   )

typedef struct {
    DATAPOOL_OPTION(OPTION_DECLARE)
} datapool_options_st;

typedef enum datapool_medium {
    DATAPOOL_MEDIUM_SHM        = 0,
    DATAPOOL_MEDIUM_PMEM       = 1
} datapool_medium_e;

struct datapool;

struct datapool *datapool_open(size_t size, int *fresh);
void datapool_close(struct datapool *pool);

void *datapool_addr(struct datapool *pool);
size_t datapool_size(struct datapool *pool);
void datapool_set_user_data(const struct datapool *pool, const void *user_data, size_t user_size);
void datapool_get_user_data(const struct datapool *pool, void *user_data, size_t user_size);
datapool_medium_e datapool_get_medium(void);
void datapool_setup(datapool_options_st *options);
void datapool_teardown(void);
