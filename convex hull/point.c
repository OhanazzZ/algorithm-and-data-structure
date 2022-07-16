#include <stdio.h>
#include <math.h>
#include "point.h"

// Returns a new Point (note, this is not a pointer to a Point)
// Since it's not allocated using dynamic memory allocation it does not
// need to be freed
Point new_point(double x, double y) {
  Point point = {x, y};
  return point;
}

// Prints (on its own line) the Point in the format "x y"
void print_point(Point p) {
  printf("%.2f %.2f\n", p.x, p.y);
}


// -----------------------below-are-functions-added---------------------------
// performs subtraction on two given vectors
Point vector_subtraction(Point p1, Point p2){
  Point p3;
  p3.x = p1.x - p2.x;
  p3.y = p1.y - p2.y;
  return p3;
}

// calculate the cross product between two vectors
double cross_product(Point p1, Point p2){
  return p1.x * p2.y - p1.y * p2.x;
}

// determines the sign of cross product, returns NEGATIVE, ZERO or POSITIVE
int determine_signed_area(double cross_product){
  if ((fabs(cross_product - 0)) < EPSILON){
    return ZERO;
  }
  else if (cross_product < 0){
    return NEGATIVE;
  }
  else{
    return POSITIVE;
  }
}
