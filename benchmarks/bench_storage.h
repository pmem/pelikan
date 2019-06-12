#pragma once

#include <cc_define.h>
#include <cc_option.h>
#include <stddef.h>

typedef size_t benchmark_key_u;

struct benchmark_entry {
    char *key;
    benchmark_key_u key_size;
    char *value;
    size_t value_size;
};

#define BENCHMARK_OPTION(ACTION)\
    ACTION(entry_min_size,  OPTION_TYPE_UINT, 64,    "Min size of cache entry")\
    ACTION(entry_max_size,  OPTION_TYPE_UINT, 64,    "Max size of cache entry")\
    ACTION(nentries,        OPTION_TYPE_UINT, 1000,  "Max total number of cache entries" )\
    ACTION(nops,            OPTION_TYPE_UINT, 100000,"Total number of operations")\
    ACTION(pct_get,         OPTION_TYPE_UINT, 80,    "% of gets")\
    ACTION(pct_put,         OPTION_TYPE_UINT, 10,    "% of puts")\
    ACTION(pct_rem,         OPTION_TYPE_UINT, 10,    "% of removes")

typedef struct {
    BENCHMARK_OPTION(OPTION_DECLARE)
} benchmark_options_st;

rstatus_i bench_storage_setup(const char *config, benchmark_options_st *options);
rstatus_i bench_storage_init(size_t item_size, size_t nentries);
rstatus_i bench_storage_deinit(void);
rstatus_i bench_storage_put(struct benchmark_entry *e);
rstatus_i bench_storage_get(struct benchmark_entry *e);
rstatus_i bench_storage_rem(struct benchmark_entry *e);
