#pragma once

// A convenient array abstraction holding void * pointers

struct vararray_t;

typedef struct vararray_t * vararray_handle;

vararray_handle vararray_create();


// Removes all the elements from the array
void vararray_clear(vararray_handle h);

// returns the number of elements in the array
unsigned int vararray_size(vararray_handle h);

// Add an element to the back of the array
void vararray_push_back(vararray_handle h, void * element);

// Access element I (0 <= I < vararray_size())
void * vararray_get(vararray_handle h, unsigned int idx);

// Destroy vararray. Does not perform any operation on the void * elements
// (in particular does not call free for these pointers)
void vararray_destroy(vararray_handle h);

