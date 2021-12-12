#include "vararray.h"

#include "stdlib.h"

// COMPLETE ME
struct vararray_t{
    void** elements;
    unsigned int size;
};
typedef struct vararray_t vararray_t;


vararray_handle vararray_create(){
    vararray_handle v = (vararray_handle)malloc(sizeof(vararray_t));
    v->elements = (void**)malloc(sizeof(void*));
    v->size = 0;
    return v;
}


void vararray_clear(vararray_handle h){
    free(h->elements);
    h->elements = (void**)malloc(sizeof(void*));
    h->size = 0;
}


unsigned int vararray_size(vararray_handle h){
    return h->size;
}


void vararray_push_back(vararray_handle h, void * element){
    if(h->size == 0){
        h->elements[0] = element;
    }
    else{
        h->elements = (void**)realloc(h->elements, sizeof(void*) * (h->size + 1));
        h->elements[h->size] = element;
    }
    h->size += 1;
}


void * vararray_get(vararray_handle h, unsigned int idx){
    return h->elements[idx];
}


void vararray_destroy(vararray_handle h){
    free(h->elements);
    free(h);
}
