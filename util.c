#include "util.h"

#include<stdlib.h>

// NOTE: I haven't been consistent on when to pass point/rectangle by pointer or by value
// (the idea behind passing by pointer is to avoid having to copy on the stack to pass it to the function,
// but in many cases the compiler can optimize away the copy anyway... especially if the function can be inlined)

// COMPLETE ME
struct valid_points{
    point_t p1;
    point_t p2;
};
typedef struct valid_points valid_points;


void point_print(point_t p, FILE * where){
    fprintf(where, "(%i,%i)", p.x, p.y);
}


point_t point_shift(point_t p, int dx, int dy){
    point_t* pp = &p;
    pp->x += dx;
    pp->y += dy;
    return *pp;
}


void rectangle_print(rectangle_t r, FILE * where){
    fprintf(where, "(%i,%i)-(%i,%i)", r.p1.x, r.p1.y, r.p2.x, r.p2.y);
}


int max(int x, int y){
    return x > y ? x : y;
}


int min(int x, int y){
    return x < y ? x : y;
}


valid_points point_ordering(point_t p1, point_t p2){
    valid_points p;
    p.p1.x = min(p1.x, p2.x);
    p.p1.y = min(p1.y, p2.y);
    p.p2.x = max(p1.x, p2.x);
    p.p2.y = max(p1.y, p2.y);
    return p;
}


rectangle_t rectangle_create(point_t p1, point_t p2){
    rectangle_t r;
    valid_points p = point_ordering(p1, p2);
    r.p1 = p.p1;
    r.p2 = p.p2;
    return r;
}


rectangle_t rectangle_combine(const rectangle_t * r1, const rectangle_t * r2){
    point_t p1, p2;
    p1.x = min(r1->p1.x, r2->p1.x);
    p2.x = max(r1->p2.x, r2->p2.x);
    p1.y = min(r1->p1.y, r2->p1.y);
    p2.y = max(r1->p2.y, r2->p2.y);
    rectangle_t r = rectangle_create(p1, p2);
    return r;
}


rectangle_t rectangle_combine_array(const rectangle_t * r, unsigned int count){
    rectangle_t r_combined = rectangle_create(r[0].p1, r[0].p2);
    for(int i = 1; i < count; i++){
        r_combined = rectangle_combine(&r_combined, (r+i));
    }
    return r_combined;
}


unsigned int rectangle_area(const rectangle_t * r){
    return (r->p2.x - r->p1.x) * (r->p2.y - r->p1.y);
}


bool rectangle_intersect(
    const rectangle_t * r1, 
    const rectangle_t * r2,
    rectangle_t * ret
){
    // printf("%i, %i, %i, %i", r1->p2.x, r2->p1.x, r1->p2.y, r2->p1.y);
    if(
        (r1->p2.x < r2->p1.x) || 
        (r2->p2.x < r1->p1.x) || 
        (r1->p2.y < r2->p1.y) ||
        (r2->p2.y < r1->p1.y)
    ){
        return false;
    }
    else{
        int p1_x = max(r1->p1.x, r2->p1.x);
        int p1_y = max(r1->p1.y, r2->p1.y);
        int p2_x = min(r1->p2.x, r2->p2.x);
        int p2_y = min(r1->p2.y, r2->p2.y);
        point_t p1 = point_create(p1_x, p1_y);
        point_t p2 = point_create(p2_x, p2_y);
        
        ret->p1 = p1;
        ret->p2 = p2;

        return true;
    }
}


unsigned int rectangle_intersect_area(
    const rectangle_t * r1,
    const rectangle_t * r2
){
    rectangle_t* intersection = (rectangle_t*)malloc(sizeof(rectangle_t));
    bool intersect = rectangle_intersect(r1, r2, intersection);
    if (intersect){
        unsigned int area = rectangle_area(intersection);
        free(intersection);
        return area;
    }
    else{
        free(intersection);
        return 0;
    }
}


bool rectangle_contains(const rectangle_t * r1, point_t p){
    if (
        (r1->p1.x <= p.x) && 
        (r1->p2.x >= p.x) && 
        (r1->p1.y <= p.y) && 
        (r1->p2.y >= p.y)
    ){
        return true;
    }
    return false;
}

// p1 is left-down, p2 is right-down, p3 is left-up, p4 is right-up
void rectangle_corners(
    const rectangle_t * r, 
    point_t * p1, 
    point_t * p2,
    point_t * p3, 
    point_t * p4
){
    p1->x = r->p1.x;
    p1->y = r->p1.y;
    p2->x = r->p2.x;
    p2->y = r->p1.y;
    p3->x = r->p1.x;
    p3->y = r->p2.y;
    p4->x = r->p2.x;
    p4->y = r->p2.y;
}


coordinate_t rectangle_min_x(const rectangle_t * r){
    return r->p1.x;
}


coordinate_t rectangle_max_x(const rectangle_t * r){
    return r->p2.x;
}


coordinate_t rectangle_min_y(const rectangle_t * r){
    return r->p1.y;
}


coordinate_t rectangle_max_y(const rectangle_t * r){
    return r->p2.y;
}


point_t rectangle_min(const rectangle_t * r){
    return r->p1;
}


point_t rectangle_max(const rectangle_t * r){
    return r->p2;
}