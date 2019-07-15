#include <datapool/datapool.h>

#include <cc_option.h>

#include <check.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SUITE_NAME "datapool"
#define DEBUG_LOG  SUITE_NAME ".log"
#define TEST_DATAFILE "./datapool.pelikan"
#define TEST_DATA_NAME "datapool_test"
#define TEST_DATASIZE (1 << 20)

datapool_options_st options = { DATAPOOL_OPTION(OPTION_INIT) };

/*
 * utilities
 */
static void
test_setup(const char *path, const char*name, bool prefault)
{
    options.datapool_path.val.vstr = path;
    options.datapool_name.val.vstr = name;
    options.datapool_prefault.val.vbool = prefault;
    datapool_setup(&options);
}

static void
test_teardown(const char* str)
{
    datapool_teardown();
    unlink(str);
}

/*
 * tests
 */
START_TEST(test_datapool)
{
    test_setup(TEST_DATAFILE, TEST_DATA_NAME, false);
    int fresh = 0;
    struct datapool *pool = datapool_open(TEST_DATASIZE, &fresh);
    ck_assert_ptr_nonnull(pool);
    size_t s = datapool_size(pool);
    ck_assert_int_ge(s, TEST_DATASIZE);
    ck_assert_int_eq(fresh, 1);
    ck_assert_ptr_nonnull(datapool_addr(pool));
    datapool_close(pool);

    pool = datapool_open(TEST_DATASIZE, &fresh);
    ck_assert_ptr_nonnull(pool);
    ck_assert_int_eq(s, datapool_size(pool));
    ck_assert_int_eq(fresh, 0);
    datapool_close(pool);
    test_teardown(TEST_DATAFILE);
}
END_TEST

START_TEST(test_devzero)
{
    test_setup(NULL, TEST_DATA_NAME, false);
    int fresh = 0;
    struct datapool *pool = datapool_open(TEST_DATASIZE, &fresh);
    ck_assert_ptr_nonnull(pool);
    size_t s = datapool_size(pool);
    ck_assert_int_ge(s, TEST_DATASIZE);
    ck_assert_int_eq(fresh, 1);
    ck_assert_ptr_nonnull(datapool_addr(pool));
    datapool_close(pool);

    pool = datapool_open(TEST_DATASIZE, &fresh);
    ck_assert_ptr_nonnull(pool);
    ck_assert_int_eq(s, datapool_size(pool));
    ck_assert_int_eq(fresh, 1);
    datapool_close(pool);
    datapool_teardown();
}
END_TEST

START_TEST(test_datapool_userdata)
{
#define MAX_USER_DATA_SIZE 2000
    char data_set[MAX_USER_DATA_SIZE] = {0};
    char data_get[MAX_USER_DATA_SIZE] = {0};
    test_setup(TEST_DATAFILE, TEST_DATA_NAME, false);
    struct datapool *pool = datapool_open(TEST_DATASIZE, NULL);
    ck_assert_ptr_nonnull(pool);
    cc_memset(data_set, 'A', MAX_USER_DATA_SIZE);
    datapool_set_user_data(pool, data_set, MAX_USER_DATA_SIZE);
    datapool_close(pool);

    pool = datapool_open(TEST_DATASIZE, NULL);
    ck_assert_ptr_nonnull(pool);
    datapool_get_user_data(pool, data_get, MAX_USER_DATA_SIZE);
    ck_assert_mem_eq(data_set, data_get, MAX_USER_DATA_SIZE);
    datapool_close(pool);
    test_teardown(TEST_DATAFILE);
#undef MAX_USER_DATA_SIZE
}
END_TEST


START_TEST(test_datapool_prealloc)
{
    test_setup(TEST_DATAFILE, TEST_DATA_NAME, true);
    struct datapool *pool = datapool_open(TEST_DATASIZE, NULL);
    ck_assert_ptr_nonnull(pool);
    datapool_close(pool);
    test_teardown(TEST_DATAFILE);
}
END_TEST

START_TEST(test_datapool_empty_signature)
{
    test_setup(TEST_DATAFILE, NULL, false);
    struct datapool *pool = datapool_open(TEST_DATASIZE, NULL);
    ck_assert_ptr_null(pool);
    datapool_teardown();
}
END_TEST

START_TEST(test_datapool_too_long_signature)
{
#define LONG_SIGNATURE "Lorem ipsum dolor sit amet, consectetur volutpat"
    test_setup(TEST_DATAFILE, LONG_SIGNATURE, false);
    struct datapool *pool = datapool_open(TEST_DATASIZE, NULL);
    ck_assert_ptr_null(pool);
    datapool_teardown();
#undef LONG_SIGNATURE
}
END_TEST

START_TEST(test_datapool_max_length_signature)
{
#define MAX_SIGNATURE "Lorem ipsum dolor sit amet, consectetur volutpa"
    test_setup(TEST_DATAFILE, MAX_SIGNATURE, false);
    struct datapool *pool = datapool_open(TEST_DATASIZE, NULL);
    ck_assert_ptr_nonnull(pool);
    datapool_close(pool);
    test_teardown(TEST_DATAFILE);
#undef MAX_SIGNATURE
}
END_TEST

START_TEST(test_datapool_wrong_signature_long_variant)
{
#define WRONG_POOL_NAME_LONG_VAR "datapool_pelikan_no_exist"
    int fresh = 0;
    test_setup(TEST_DATAFILE, TEST_DATA_NAME, false);
    struct datapool *pool = datapool_open(TEST_DATASIZE, &fresh);
    ck_assert_ptr_nonnull(pool);
    size_t s = datapool_size(pool);
    ck_assert_int_ge(s, TEST_DATASIZE);
    ck_assert_int_eq(fresh, 1);
    ck_assert_ptr_nonnull(datapool_addr(pool));
    datapool_close(pool);
    datapool_teardown();

    test_setup(TEST_DATAFILE, WRONG_POOL_NAME_LONG_VAR, false);
    pool = datapool_open(TEST_DATASIZE, NULL);
    ck_assert_ptr_null(pool);
    test_teardown(TEST_DATAFILE);
#undef WRONG_POOL_NAME_LONG_VAR
}
END_TEST

START_TEST(test_datapool_wrong_signature_short_variant)
{
#define WRONG_POOL_NAME_SHORT_VAR "datapool"
    int fresh = 0;
    test_setup(TEST_DATAFILE, TEST_DATA_NAME, false);
    struct datapool *pool = datapool_open(TEST_DATASIZE, &fresh);
    ck_assert_ptr_nonnull(pool);
    size_t s = datapool_size(pool);
    ck_assert_int_ge(s, TEST_DATASIZE);
    ck_assert_int_eq(fresh, 1);
    ck_assert_ptr_nonnull(datapool_addr(pool));
    datapool_close(pool);
    datapool_teardown();

    test_setup(TEST_DATAFILE, WRONG_POOL_NAME_SHORT_VAR, false);
    pool = datapool_open(TEST_DATASIZE, NULL);
    ck_assert_ptr_null(pool);
    test_teardown(TEST_DATAFILE);
#undef WRONG_POOL_NAME_SHORT_VAR
}
END_TEST

/*
 * test suite
 */
static Suite *
datapool_suite(void)
{
    Suite *s = suite_create(SUITE_NAME);

    TCase *tc_pool = tcase_create("pool");
    tcase_add_test(tc_pool, test_datapool);
    tcase_add_test(tc_pool, test_devzero);
    tcase_add_test(tc_pool, test_datapool_userdata);
    tcase_add_test(tc_pool, test_datapool_prealloc);
    tcase_add_test(tc_pool, test_datapool_max_length_signature);
    tcase_add_test(tc_pool, test_datapool_empty_signature);
    tcase_add_test(tc_pool, test_datapool_too_long_signature);
    tcase_add_test(tc_pool, test_datapool_wrong_signature_short_variant);
    tcase_add_test(tc_pool, test_datapool_wrong_signature_long_variant);

    suite_add_tcase(s, tc_pool);

    return s;
}

int
main(void)
{
    int nfail;

    Suite *suite = datapool_suite();
    SRunner *srunner = srunner_create(suite);
    srunner_set_fork_status(srunner, CK_NOFORK);
    srunner_set_log(srunner, DEBUG_LOG);
    srunner_run_all(srunner, CK_ENV); /* set CK_VEBOSITY in ENV to customize */
    nfail = srunner_ntests_failed(srunner);
    srunner_free(srunner);

    return (nfail == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
