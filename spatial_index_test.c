#include "spatial_index.h"
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


bool si_intersects_func_func(const rectangle_t * p, const void * element, void * userdata){
    rectangle_t* rec_element = (rectangle_t*)element;
    rectangle_t* intersect = (rectangle_t*)malloc(sizeof(rectangle_t));
    // rectangle_print(*p, stdout);
    // rectangle_print(*rec_element, stdout);
    bool status = rectangle_intersect(p, rec_element, intersect);
    printf("%i", status);
    free(intersect);
    return status;
}


void si_bounding_box_func(const void * element, rectangle_t * rect, void * userdata){
    rectangle_t* rec_element = (rectangle_t*)element;
    
    point_t p1 = rectangle_min(rec_element);
    point_t p2 = rectangle_max(rec_element);
    rect->p1 = p1;
    rect->p2 = p2;
}


bool si_visit_func(void * element, void * userdata){
    rectangle_t* rec_element = (rectangle_t*)element;
    rectangle_print(*rec_element, stdout);
    return true;
}


static void test_si_create(){
    si_handle si = si_create(
        si_intersects_func_func, 
        si_bounding_box_func,
        NULL);
    
    CU_ASSERT_NOT_EQUAL(si, NULL);
    CU_ASSERT_EQUAL(si_size(si), 0);
    si_destroy(si);
}


static void test_si_insert_remove_walk_size(){
    si_handle si = si_create(
        si_intersects_func_func, 
        si_bounding_box_func,
        NULL);
    
    si_walk(si, si_visit_func, NULL);

    // (2,2)-(3,3)
    coordinate_t x1 = 2;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 3;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);

    si_insert(si, &r1);
    // si_walk(si, si_visit_func, NULL);
    CU_ASSERT_EQUAL(si_size(si), 1);

    // (1,2)-(3,5)
    x1 = 1;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 5;
    p2 = point_create(x1, y1);
    rectangle_t r2 = rectangle_create(p2, p1);
    si_insert(si, &r2);
    CU_ASSERT_EQUAL(si_size(si), 2);

    // (4,4)-(6,5)
    x1 = 4;
    y1 = 4;
    p1 = point_create(x1, y1);
    x1 = 6;
    y1 = 5;
    p2 = point_create(x1, y1);
    rectangle_t r3 = rectangle_create(p2, p1);

    si_insert(si, &r3);
    si_walk(si, si_visit_func, NULL);
    CU_ASSERT_EQUAL(si_size(si), 3);

    printf("\n");
    bool status = si_remove(si, &r2);
    si_walk(si, si_visit_func, NULL);
    CU_ASSERT_EQUAL(si_size(si), 2);
    CU_ASSERT_EQUAL(status, true);

    printf("\n");
    status = si_remove(si, &r3);
    si_walk(si, si_visit_func, NULL);
    CU_ASSERT_EQUAL(si_size(si), 1);
    CU_ASSERT_EQUAL(status, true);

    // (0,0)-(1,1)
    printf("\n");
    x1 = 4;
    y1 = 4;
    p1 = point_create(x1, y1);
    x1 = 6;
    y1 = 5;
    p2 = point_create(x1, y1);
    rectangle_t r4 = rectangle_create(p2, p1);
    status = si_remove(si, &r4);
    si_walk(si, si_visit_func, NULL);
    CU_ASSERT_EQUAL(si_size(si), 1);
    CU_ASSERT_EQUAL(status, false);

    printf("\n");
    status = si_remove(si, &r1);
    CU_ASSERT_EQUAL(si_size(si), 0);
    CU_ASSERT_EQUAL(status, true);

    si_destroy(si);
}


static void test_si_bbox(){
    si_handle si = si_create(
        si_intersects_func_func, 
        si_bounding_box_func,
        NULL);
    
    rectangle_t bbox;
    bool status = si_bbox(si, &bbox);
    CU_ASSERT_EQUAL(status, false);

    coordinate_t x1 = 2;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 3;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);
    si_insert(si, &r1);

    x1 = 1;
    y1 = 2;
    p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 5;
    p2 = point_create(x1, y1);
    rectangle_t r2 = rectangle_create(p2, p1);
    si_insert(si, &r2);

    status = si_bbox(si, &bbox);
    CU_ASSERT_EQUAL(status, true);

    x1 = 5;
    y1 = 5;
    p1 = point_create(x1, y1);
    x1 = 7;
    y1 = 6;
    p2 = point_create(x1, y1);
    rectangle_t r3 = rectangle_create(p2, p1);
    si_insert(si, &r3);

    status = si_bbox(si, &bbox);
    CU_ASSERT_EQUAL(status, true);
    rectangle_print(bbox, stdout);

    si_destroy(si);
}


static void test_si_intersect(){
    si_handle si = si_create(
        si_intersects_func_func, 
        si_bounding_box_func,
        NULL);
    
    coordinate_t x1 = 2;
    coordinate_t y1 = 2;
    point_t p1 = point_create(x1, y1);
    x1 = 3;
    y1 = 3;
    point_t p2 = point_create(x1, y1);
    rectangle_t r1 = rectangle_create(p2, p1);
    si_insert(si, &r1);

    x1 = 1;
    y1 = 1;
    p1 = point_create(x1, y1);
    x1 = 2;
    y1 = 2;
    p2 = point_create(x1, y1);
    rectangle_t r2 = rectangle_create(p2, p1);
    si_insert(si, &r2);

    x1 = 3;
    y1 = 3;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 4;
    p2 = point_create(x1, y1);
    rectangle_t r3 = rectangle_create(p2, p1);
    si_insert(si, &r3);
    si_walk(si, si_visit_func, NULL);

    x1 = 0;
    y1 = 0;
    p1 = point_create(x1, y1);
    x1 = 2;
    y1 = 2;
    p2 = point_create(x1, y1);
    rectangle_t r4 = rectangle_create(p2, p1);
    unsigned int intersecting_num = si_intersect(
        si,
        &r4,
        si_visit_func,
        NULL
    );
    // printf("%i\n", intersecting_num);
    CU_ASSERT_EQUAL(intersecting_num, 2);

    x1 = 1;
    y1 = 1;
    p1 = point_create(x1, y1);
    x1 = 4;
    y1 = 4;
    p2 = point_create(x1, y1);
    rectangle_t r5 = rectangle_create(p2, p1);
    intersecting_num = si_intersect(
        si,
        &r5,
        si_visit_func,
        NULL
    );
    // printf("%i\n", intersecting_num);
    CU_ASSERT_EQUAL(intersecting_num, 3);

    si_destroy(si);
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
        (NULL == CU_add_test(pSuite, "si_create", test_si_create)) ||
        (NULL == CU_add_test(pSuite, "si_insert_remove_walk_size", test_si_insert_remove_walk_size)) ||
        (NULL == CU_add_test(pSuite, "si_bbox", test_si_bbox)) || 
        (NULL == CU_add_test(pSuite, "si_intersect", test_si_intersect))
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