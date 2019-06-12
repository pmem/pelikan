#include <bench_storage.h>

#include <storage/cuckoo/item.h>
#include <storage/cuckoo/cuckoo.h>

static cuckoo_metrics_st metrics = { CUCKOO_METRIC(METRIC_INIT) };

struct bench_storage_options {
    benchmark_options_st benchmark;
    cuckoo_options_st cuckoo;
};

static struct bench_storage_options bench_all_opt =
{
    { BENCHMARK_OPTION(OPTION_INIT) },
    { CUCKOO_OPTION(OPTION_INIT) }
};

rstatus_i
bench_storage_setup(const char *config, benchmark_options_st *options)
{
    FILE *fp;
    rstatus_i status;

    unsigned int nopts = OPTION_CARDINALITY(struct bench_storage_options);

    status = option_load_default((struct option *)&bench_all_opt, nopts);

    if (config != NULL) {
        fp = fopen(config, "r");
        if (fp == NULL) {
            log_crit("failed to open the config file");
            return CC_ERROR;
        }
        status = option_load_file(fp, (struct option *)&bench_all_opt, nopts);
        fclose(fp);
    }

    cc_memcpy(options, &bench_all_opt.benchmark, sizeof(benchmark_options_st));

    return status;
}

rstatus_i
bench_storage_init(size_t item_size, size_t nentries)
{
    bench_all_opt.cuckoo.cuckoo_policy.val.vuint = CUCKOO_POLICY_EXPIRE;
    bench_all_opt.cuckoo.cuckoo_item_size.val.vuint = item_size + ITEM_OVERHEAD;
    bench_all_opt.cuckoo.cuckoo_nitem.val.vuint = nentries;

    cuckoo_setup(&bench_all_opt.cuckoo, &metrics);

    return CC_OK;
}

rstatus_i
bench_storage_deinit(void)
{
    cuckoo_teardown();
    return CC_OK;
}

rstatus_i
bench_storage_put(struct benchmark_entry *e)
{
    struct bstring key;
    struct val val;
    val.type = VAL_TYPE_STR;
    bstring_set_cstr(&val.vstr, e->value);
    bstring_set_cstr(&key, e->key);

    struct item *it = cuckoo_insert(&key, &val, INT32_MAX);

    return it != NULL ? CC_OK : CC_ENOMEM;
}

rstatus_i
bench_storage_get(struct benchmark_entry *e)
{
    struct bstring key;
    bstring_set_cstr(&key, e->key);
    struct item *it = cuckoo_get(&key);

    return it != NULL ? CC_OK : CC_EEMPTY;
}

rstatus_i
bench_storage_rem(struct benchmark_entry *e)
{
    struct bstring key;
    bstring_set_cstr(&key, e->key);

    return cuckoo_delete(&key) ? CC_OK : CC_EEMPTY;
}
