#include "spatial_index.h"
#include "util.h"

#include <assert.h>
#include <stdlib.h>

// COMPLETE ME
struct spatial_element{
    void* element;
    struct spatial_element* next;
};
typedef struct spatial_element spatial_element;


struct spatial_index_t{
    spatial_element* first_element;
    unsigned int num_element;
    si_intersects_func intersects_func;
    si_bounding_box bounding_box_func;
    void* userdata;
};
typedef struct spatial_index_t spatial_index_t;


si_handle si_create(
    si_intersects_func contains, 
    si_bounding_box box, 
    void * userdata
){
    si_handle si = (si_handle)malloc(sizeof(spatial_index_t));
    si->first_element = NULL;
    si->num_element = 0;
    si->intersects_func = contains;
    si->bounding_box_func = box;
    si->userdata = userdata;

    return si;
}


bool si_walk(const_si_handle si, si_visit visitfunc, void * userdata){
    spatial_element* cur = si->first_element;
    for (int i = 0; i < si->num_element; i++){
        if (!visitfunc(cur->element, userdata)){
            return false;
        }
        cur = cur->next;
    }
    return true;
}


spatial_element* get_last_element(si_handle si){
    spatial_element* cur = si->first_element;
    for (int i = 0; i < si->num_element-1; i++){
        cur = cur->next;
    }
    return cur;
}


void si_insert(si_handle si, void * element){
    spatial_element* new_element = (spatial_element*)malloc(sizeof(spatial_element));
    new_element->element = element;
    new_element->next = NULL;

    if (si->num_element == 0){
        si->first_element = new_element;
    }
    else{
        spatial_element* last = get_last_element(si);
        last->next = new_element;
    }
    si->num_element += 1;
}


bool si_remove(si_handle si, void * element){
    if (si->num_element == 0){
        return false;
    }
    spatial_element* pre;
    spatial_element* cur = si->first_element;
    for (int i = 0; i < si->num_element; i++){
        if (cur->element == element){
            if (i == 0){
                si->first_element = cur->next;
            }
            else{
                pre->next = cur->next;
            }
            free(cur);
            si->num_element -= 1;
            return true;
        }
        pre = cur;
        cur = cur->next;
    }
    return false;
}


unsigned int si_size(const_si_handle si){
    return si->num_element;
}


bool si_bbox(const_si_handle si, rectangle_t * rect){
    if (si->num_element == 0){
        return false;
    }

    rectangle_t* rectangle_in_index = (rectangle_t*)malloc(sizeof(rectangle_t)*si->num_element);
    spatial_element* cur = si->first_element;
    for (int i = 0; i < si->num_element; i++){
        rectangle_t rec;
        si->bounding_box_func(cur->element, &rec, NULL);
        rectangle_in_index[i] = rec;
        cur = cur->next;
    }
    rectangle_t r = rectangle_combine_array(rectangle_in_index, si->num_element);
    rect->p1 = r.p1;
    rect->p2 = r.p2;
    free(rectangle_in_index);
    return true;
}


unsigned int si_intersect(
    const_si_handle si, 
    const rectangle_t * rect, 
    si_visit visitfunc,
    void * userdata
){
    if (si->num_element == 0){
        return 0;
    }
    else{
        int num = 0;
        spatial_element* cur = si->first_element;
        for (int i = 0; i < si->num_element; i++){
            if (si->intersects_func(rect, cur->element, NULL)){
                num += 1;
            }
            cur = cur->next;
        }
        return num;
    }
}


void si_destroy(si_handle si){
    if (si != NULL){
        spatial_element* cur = si->first_element;
        spatial_element* next;
        for (int i = 0; i < si->num_element; i++){
            next = cur->next;
            free(cur);
            cur = next;
        }
        free(si);
    }
}