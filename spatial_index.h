#pragma once

#include "util.h"


struct spatial_index_t;

typedef struct spatial_index_t * si_handle;
typedef const struct spatial_index_t * const_si_handle;


// Function which returns true if the element intersects with any points of the provided
// rectangle.
// (userdata is extra data the passed in by the client, and passed through unmodified to the
// contains_func.
// The function should not make any change to element which would affect the bounding box/dimensions
// of the element!
typedef bool (*si_intersects_func) (const rectangle_t * p, const void * element, void * userdata);

// Function which will set *rect to the smallest rectangle fully enclosing the given element.
// The function should not make any change to element which would affect the bounding box/dimensions
// of the element!
typedef void (*si_bounding_box) (const void * element, rectangle_t * rect, void * userdata);

typedef bool (*si_visit) (void * element, void * userdata);

// Create a new spatial index
//   IMPORTANT: There should be NO HARDCODED LIMIT on how many elements can be stored in the index!
si_handle si_create(si_intersects_func contains, si_bounding_box box, void *
        userdata);


// Enumerate the contents of the spatial index. For each element in the index, calls
// the provided function (passing through userdata unmodified). The order in which the elements
// are enumerated is not specified.
//
// If the function returns false, enumeration stops immediately.
//
// The return value of the si_walk function is:
//    - true if the user function returned true for every element of the index
//      --or-- there are no elements in the index.
//    - false if the user function returned false for an element.
//
bool si_walk(const_si_handle si, si_visit visitfunc, void * userdata);

// Add an element to the spatial index
void si_insert(si_handle si, void * element);

// Remove an element from the index
// Returns true if the element was found (and removed), false otherwise.
bool si_remove(si_handle si, void * element);

// Returns the number of elements in the index
unsigned int si_size(const_si_handle si);

// Returns the bounding box capable of containing all elements in the index.
// Returns false if no elements are in the index, true otherwise.
bool si_bbox(const_si_handle si, rectangle_t * rect);

// Calls a function for every element which overlaps with the given rectangle or point
// (for a point, specify a rectangle where p1 == p2). Returns the number of elements found.
unsigned int si_intersect(const_si_handle si, const rectangle_t * rect, si_visit visitfunc,
    void * userdata);

// Destroy a spatial index.
// This function does not do anything to any elements stored within the index
// (in particular, *it does not call free on them*)
void si_destroy(si_handle si);

