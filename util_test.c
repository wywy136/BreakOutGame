#include "util.h"

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


static void test_point(){
    coordinate_t x1 = 1;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    point_print(p1, stdout);

    point_t p2 = point_create(x1, y1);
    point_print(p2, stdout);

    bool equal = point_equal(p1, p2);
    CU_ASSERT_EQUAL(equal, true);

    p2 = point_shift(p1, 0, 1);
    point_print(p2, stdout);

    equal = point_equal(p1, p2);
    CU_ASSERT_EQUAL(equal, false);
}


static void test_rectangle_create(){
    coordinate_t x1 = 1;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);

    x1 = 3;
    y1 = 3;
    point_t p2 = point_create(x1, y1);

    rectangle_t r = rectangle_create(p2, p1);
    rectangle_print(r, stdout);
}


static void test_rectangle_equal_combine(){
    coordinate_t x1 = 1;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 3;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);
    rectangle_print(r1, stdout);
    
    x1 = 3;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 0;
    p2 = point_create(x1, y1);
    rectangle_t r2 = rectangle_create(p1, p2);
    rectangle_print(r2, stdout);

    bool equal = rectangle_equal(&r1, &r2);
    CU_ASSERT_EQUAL(equal, false);

    rectangle_t r_c = rectangle_combine(&r1, &r2);
    rectangle_print(r_c, stdout);

    x1 = 4;
    y1 = 3;
    p1 = point_create(x1, y1);
    x1 = 5;
    y1 = 4;
    p2 = point_create(x1, y1);
    rectangle_t r3 = rectangle_create(p1, p2);
    rectangle_print(r3, stdout);

    rectangle_t r[3];
    r[0] = r1;
    r[1] = r2;
    r[2] = r3;
    rectangle_t r_c1 = rectangle_combine_array(r, 3);
    rectangle_print(r_c1, stdout);
}


static void test_rectangle_area(){
    coordinate_t x1 = 1;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 3;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);
    CU_ASSERT_EQUAL(rectangle_area(&r1), 2);
    
    x1 = 3;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 6;
    p2 = point_create(x1, y1);
    rectangle_t r2 = rectangle_create(p1, p2);
    CU_ASSERT_EQUAL(rectangle_area(&r2), 0);
}


static void test_rectangle_intersect(){
    // (1,1)-(3,3) * (2,2)-(4,4) => 1
    coordinate_t x1 = 2;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 3;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);
    x1 = 0;
    y1 = 0;
    p1 = point_create(x1, y1);
    x1 = 2;
    y1 = 2;
    p2 = point_create(x1, y1);
    rectangle_t r2 = rectangle_create(p1, p2);
    rectangle_t* intersect = (rectangle_t*)malloc(sizeof(rectangle_t));
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), true);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 0);

    // (4,4)-(2,2) * (3,3)-(1,1) => 1
    x1 = 2;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 4;
    p2 = point_create(x1, y1);
    r1 = rectangle_create(p2, p1);
    x1 = 3;
    y1 = 3;
    p1 = point_create(x1, y1);
    x1 = 1;
    y1 = 1;
    p2 = point_create(x1, y1);
    r2 = rectangle_create(p1, p2);
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), true);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 1);

    // (2,2)-(4,4) * (1,2)-(3,5) => 2
    x1 = 2;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 4;
    p2 = point_create(x1, y1);
    r1 = rectangle_create(p2, p1);
    x1 = 1;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 5;
    p2 = point_create(x1, y1);
    r2 = rectangle_create(p1, p2);
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), true);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 2);

    // (1,2)-(3,5) * (2,2)-(4,4)=> 2
    x1 = 1;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 5;
    p2 = point_create(x1, y1);
    r1 = rectangle_create(p2, p1);
    x1 = 2;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 4;
    p2 = point_create(x1, y1);
    r2 = rectangle_create(p1, p2);
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), true);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 2);

    // (1,1)-(3,3) * (1,1)-(3,3)=> 4
    x1 = 1;
    y1 = 1;
    p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 3;
    p2 = point_create(x1, y1);
    r1 = rectangle_create(p2, p1);
    r2 = rectangle_create(p1, p2);
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), true);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 4);

    // (1,1)-(5,5) * (2,2)-(4,4)=> 4
    x1 = 1;
    y1 = 1;
    p1 = point_create(x1, y1);
    x1 = 5;
    y1 = 5;
    p2 = point_create(x1, y1);
    r1 = rectangle_create(p2, p1);
    x1 = 2;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 4;
    p2 = point_create(x1, y1);
    r2 = rectangle_create(p1, p2);
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), true);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 4);

    // (2,2)-(4,4) * (1,1)-(5,5)=> 4
    x1 = 2;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 4;
    p2 = point_create(x1, y1);
    r1 = rectangle_create(p2, p1);
    x1 = 1;
    y1 = 1;
    p1 = point_create(x1, y1);
    x1 = 5;
    y1 = 5;
    p2 = point_create(x1, y1);
    r2 = rectangle_create(p1, p2);
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), true);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 4);

    // (2,2)-(4,4) * (6,6)-(7,8)=> 0
    x1 = 2;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 4;
    p2 = point_create(x1, y1);
    r1 = rectangle_create(p2, p1);
    x1 = 6;
    y1 = 6;
    p1 = point_create(x1, y1);
    x1 = 7;
    y1 = 8;
    p2 = point_create(x1, y1);
    r2 = rectangle_create(p1, p2);
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), false);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 0);

    // (2,2)-(6,4) * (1,1)-(4,2)=> 0
    x1 = 2;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 6;
    y1 = 6;
    p2 = point_create(x1, y1);
    r1 = rectangle_create(p2, p1);
    x1 = 1;
    y1 = 1;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 2;
    p2 = point_create(x1, y1);
    r2 = rectangle_create(p1, p2);
    rectangle_print(r1, stdout);
    rectangle_print(r2, stdout);

    CU_ASSERT_EQUAL(rectangle_intersect(&r1, &r2, intersect), true);
    CU_ASSERT_EQUAL(rectangle_intersect_area(&r1, &r2), 0);

    free(intersect);
}


static void test_rectangle_contains(){
    coordinate_t x1 = 2;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 5;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);
    CU_ASSERT_EQUAL(rectangle_contains(&r1, p2), true);

    p2 = point_create(3, 4);
    CU_ASSERT_EQUAL(rectangle_contains(&r1, p2), true);

    p2 = point_create(6, 6);
    CU_ASSERT_EQUAL(rectangle_contains(&r1, p2), false);
}


static void test_rectangle_corners(){
    point_t p[4];
    coordinate_t x1 = 2;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 5;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);

    rectangle_corners(&r1, &p[0], &p[1], &p[2], &p[3]);
    for(int i = 0; i < 4; i++){
        point_print(p[i], stdout);
    }
}


static void test_rectangle_min_max(){
    coordinate_t x1 = 2;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 5;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);

    CU_ASSERT_EQUAL(rectangle_min_x(&r1), 2);
    CU_ASSERT_EQUAL(rectangle_max_x(&r1), 4);
    CU_ASSERT_EQUAL(rectangle_min_y(&r1), 2);
    CU_ASSERT_EQUAL(rectangle_max_y(&r1), 5);

    point_t min_p = rectangle_min(&r1);
    point_t max_p = rectangle_max(&r1);
    point_print(min_p, stdout);
    point_print(max_p, stdout);
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
        (NULL == CU_add_test(pSuite, "point_create", test_point)) || 
        (NULL == CU_add_test(pSuite, "rectangle_create", test_rectangle_create)) ||
        (NULL == CU_add_test(pSuite, "rectangle_equal_combine", test_rectangle_equal_combine)) ||
        (NULL == CU_add_test(pSuite, "rectangle_area", test_rectangle_area)) || 
        (NULL == CU_add_test(pSuite, "rectangle_intersect", test_rectangle_intersect)) ||
        (NULL == CU_add_test(pSuite, "rectangle_contains", test_rectangle_contains)) || 
        (NULL == CU_add_test(pSuite, "rectangle_corners", test_rectangle_corners)) ||
        (NULL == CU_add_test(pSuite, "rectangle_min_max", test_rectangle_min_max))
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