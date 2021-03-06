#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include "point.h"

// Possible results from the inside_hull() algorithm
#define INSIDE_HULL_ERROR -1
#define NO_INSIDE_HULL_ERROR 1
#define COLLINEAR_POINTS -2
#define NO_COLLINEAR_POINTS 2

// The possible orientation() return values
#define LEFT 'l'
#define RIGHT 'r'
#define COLLINEAR 'c'

// Returns the orientation of Point p2 in relation to the line segment p0p1.
// If p2 is to the left of p0p1 then it returns LEFT ('l'), if p2 is to the
// right it returns RIGHT ('r').
// If p0, p1 and p2 are collinear then COLLINEAR ('c') is returned.
char orientation(Point p0, Point p1, Point p2);

// Takes a polygon (i.e. an array of points) given in counter-clockwise order
// with n points.
//
// Stores the points of the convex hull into the hull array (the last point
// should NOT be the same as the first point), and returns the number of
// points which are in the convex hull.
//
// If three successive points in the polygon are collinear then the algorithm
// should terminate and COLLINEAR_POINTS should be returned.
//
// If an error occurs this function should return INSIDE_HULL_ERROR.
int inside_hull(Point *polygon, int n, Point *hull);


// -----------------------below-are-functions-added---------------------------
// helper function to detect collinearity in an array of points
int test_collinearity(Point *points, int size);

#endif
