#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef int coordinate_t;

// Coordinate system.
//   x axis is incrementing to the right, y axis is incrementing upwards.
//   (0,0) is down and left of (1,1)
typedef struct
{
	coordinate_t x;
	coordinate_t y;
} point_t;


static inline point_t point_create(coordinate_t x, coordinate_t y)
{
	point_t t;
	t.x = x;
	t.y = y;
	return t;
}

// The points are *inclusive*; i.e. the rectangle ( (0,0), (1,1) ) has area 1
// IMPORTANT: rectangles are REQUIRED to be specified by the two points in the
// rectangle which minimize and maximize both X and Y respectively.
//
// P1 *must* be the point which minimizes the coordinates, and P2 *must* be
// the point in the rectangle which has the maximal X and Y coordinate.
//
// NOTE: this struct is in the header but not meant for direct user access.
// Use the rectangle_xxx functions to manipulate.
//
typedef struct
{
	point_t p1;
	point_t p2;
} rectangle_t;

static inline point_t rectangle_get_p1(const rectangle_t * r)
{
	return r->p1;
}

static inline point_t rectangle_get_p2(const rectangle_t * r)
{
	return r->p2;
}

// Print point to the screen (using printf): format (x,y): example (0,1)
// NOTE: do not add a newline
void point_print(point_t p, FILE * where);

// Adjust the coordinate of a point by the specified x and y offset.
// Returns the adjusted point
point_t point_shift(point_t p, int dx, int dy);

// C doesn't provide operator == for structs
static inline bool point_equal(point_t p1, point_t p2)
{
	return ((p1.x == p2.x) && (p1.y == p2.y));
}

static inline bool rectangle_equal(const rectangle_t * r1, const rectangle_t * r2)
{
	return ((point_equal(r1->p1, r2->p1)) && (point_equal(r1->p2, r2->p2)));
}

// Print out the lower left and upper right point separated by '-'
// I.e.  (0,0)-(1,1)
// Note: do not add a newline!
void rectangle_print(rectangle_t r, FILE * where);

// SEE THE NOTE ABOUT THE ORDERING OF THE POINTS!
//
// This function should take points in *any* order but create rectangles
// which satisfy the requirement.
//
// NOTE: Degenerate rectangles (both point and lines) should be accepted.
//
// I strongly suggest you write a function to validate this,
// and call it liberally whenever you create or modify a rectangle,
// (as a postcondition) or are passed a rectangle (as a precondition),
// to ensure the ordering assumption is never violated
// (as this will make debugging your code, in particular the
//  rectangle_intersect function extremely difficult!)
rectangle_t rectangle_create(point_t p1, point_t p2);

// Return the rectangle including both input rectangles
rectangle_t rectangle_combine(const rectangle_t * r1, const rectangle_t * r2);

// Return the rectangle containing all the rectangles in the specified array
rectangle_t rectangle_combine_array(const rectangle_t * r, unsigned int count);

// Return area of rectangle
// The area of degenerate rectangles (both points and lines) is 0.
unsigned int rectangle_area(const rectangle_t * r);

// Return the intersection of two rectangles, *if* they intersect.
// Returns true if they do, and stores the rectangle representing the
// intersection in *ret.
// Returns false otherwise
//
// NOTE: make sure to test this properly. There are at many different cases:
//   r1 contained within r2, r2 contained within r1,
//   r1 same as r2,
//   r1 not intersecting r2,
//   r1 top left of r2, r2, top left of r1,
//   r1 top right of r2, ...
//   r1 bottom left of r2, ...
//
// NOTE for your unit test: the following should always be true for I the
// intersection of R1 and R2
//   If intersection:
//    - every point of I should be within either R1 or R2.  (and of course,
//      being rectangles, checking the corner points should be sufficient)
//    - a point 1 pixel outside of I (left right top bottom) should not be in
//     either R1, R2 or both.
//   If no intersection:
//     - none of the points of R1 should be within R2 (and the other way around).
//     (sufficient to test corners)
//
//  You can write a unit test which generates R1 and R2, and then validates
//  that I satisfies the conditions listed above.
//
//  If you make the search space small enough, you can probably try *all*
//  possible rectangle intersections in that space.
//
//  For example, assume a 5x5 grid. Generate all possible rectangles in this
//  space?  Clearly, there are 5x5 = 25 possible points for the bottom left
//  corner.  For a second point, we can't pick anything to the left or down of
//  the first point, so there are less than 25 possible points for the second
//  coordinate of the rectangle.
//
//    So the total number of rectangles in this space is less than 25x25.  The
//    number of possible pairs (to calculate the intersection of), assuming
//    ordering matters, is then (25x25) x (25x25) ~= 400,000 which should not
//    be a problem to test.
bool rectangle_intersect(const rectangle_t * r1, const rectangle_t * r2,
        rectangle_t * ret);

// Return area of intersection of two rectangles (can be 0 if the rectangles
// don't intersect)
unsigned int rectangle_intersect_area(const rectangle_t * r1,
        const rectangle_t * r2);

bool rectangle_contains(const rectangle_t * r1, point_t p);

// Get the corners of a rectangle
void rectangle_corners(const rectangle_t * r, point_t * p1, point_t * p2,
        point_t * p3, point_t * p4);

// Return the minimum/max X/Y coordinate of any point in the rectangle
coordinate_t rectangle_min_x(const rectangle_t * r);
coordinate_t rectangle_max_x(const rectangle_t * r);
coordinate_t rectangle_min_y(const rectangle_t * r);
coordinate_t rectangle_max_y(const rectangle_t * r);

// Return the point with the minimum or maximum X&Y coordinate in the rectangle
point_t rectangle_min(const rectangle_t * r);
point_t rectangle_max(const rectangle_t * r);
