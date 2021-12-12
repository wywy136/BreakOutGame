#include "vararray.h"
#include "util.h"
#include "spatial_index.h"

#include <CUnit/Basic.h>

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>


static int init_suite(void)
{
   return 0;
}

static int clean_suite(void)
{
   return 0;
}


static void test_vararray(){
    vararray_handle va = vararray_create();

    coordinate_t x1 = 2;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 3;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);

    vararray_push_back(va, &r1);
    CU_ASSERT_EQUAL(vararray_size(va), 1);

    x1 = 1;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 5;
    p2 = point_create(x1, y1);
    rectangle_t r2 = rectangle_create(p2, p1);

    vararray_push_back(va, &r2);
    CU_ASSERT_EQUAL(vararray_size(va), 2);

    rectangle_t* r = vararray_get(va, 1);
    rectangle_print(*r, stdout);

    vararray_clear(va);
    CU_ASSERT_EQUAL(vararray_size(va), 0);

    vararray_destroy(va);
}


int main()
{
    srand(time(NULL));
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (NULL == CU_add_test(pSuite, "vararray", test_vararray))
    )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}