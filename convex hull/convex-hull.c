#include <stdio.h>
#include <stdlib.h>

#include "convex-hull.h"
#include "deque.h"
#include <assert.h>


// Returns the orientation of Point p2 in relation to the line segment p0p1.
// If p2 is to the left of p0p1 then it returns LEFT ('l'), if p2 is to the
// right it returns RIGHT ('r').
// If p0, p1 and p2 are collinear then COLLINEAR ('c') is returned.
char orientation(Point p0, Point p1, Point p2) {
  // p0_p1 is the vector that goes from p0 to p1
  // p0_p2 is the vector that goes from p0 to p2
  Point p0_p1 = vector_subtraction(p1, p0), p0_p2 = vector_subtraction(p2, p0);
  int signed_area = determine_signed_area(cross_product(p0_p1, p0_p2));

  // following the definition of signed area given in the appendix and using
  // it to determine the relative position of three given vectors.
  if (signed_area == ZERO){
    return COLLINEAR;
  }
  else if (signed_area == NEGATIVE){
    return RIGHT;
  }
  else{
    return LEFT;
  }
}

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
int inside_hull(Point *polygon, int n, Point *hull) {
  Deque *deque = new_deque();
  assert(deque);
  int polygon_size = n;

  // if the first three points are collinear then return and exit to main
  if (test_collinearity(polygon, polygon_size) == COLLINEAR_POINTS){
    return COLLINEAR_POINTS;
  }
  else{
    initialise(deque, polygon);
  }

  int position;
  // first 3 points are included already, so starts construction at the 4th
  for (position = FORTH_POINT; position < polygon_size; position++){
    if (construct(deque, polygon[position]) == INSIDE_HULL_ERROR){
      return INSIDE_HULL_ERROR;
    }
  }

  // Construction was successful! Now copy points from Deque to hull
  Node *next = deque->bottom;
  // hull has one less point than Deque, hence (size -1)
  for (position = FIRST_POINT ; position < (deque->size - 1); position++){
    hull[position] = next->data;
    next = next->to_top;
  }

  // free pointer
  free_deque(deque);

  // hull has one less point than Deque, hence (size -1)
  return deque->size - 1;
}


// -----------------------below-are-functions-added---------------------------
// helper function to detect collinearity in an array of points
int test_collinearity(Point *points, int size){
  int last_point = size - NEXT_POINT, second_last_point = size - NEXT_2_POINT;
  int position;

  // check collinearity of points within the array
  // 'size - 2' because the function compares three points at a time
  for (position = FIRST_POINT; position < second_last_point; position++){
    if (orientation(points[position], points[position + NEXT_POINT],
      points[position + NEXT_2_POINT]) == COLLINEAR){
        return COLLINEAR_POINTS;
    }
  }

  // check collinearity of points on the array boundary
  if (orientation(points[last_point], points[FIRST_POINT],points[SECOND_POINT])
  == COLLINEAR || orientation(points[second_last_point], points[last_point],
    points[FIRST_POINT]) == COLLINEAR){
    return COLLINEAR_POINTS;
  }

  return NO_COLLINEAR_POINTS;
}
