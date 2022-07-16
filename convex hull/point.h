#ifndef POINT_H
#define POINT_H

#define POSITIVE 1
#define NEGATIVE -1
#define ZERO 0
#define EPSILON 0.001
#define FIRST_POINT 0
#define SECOND_POINT 1
#define THIRD_POINT 2
#define FORTH_POINT 3
#define NEXT_POINT 1
#define NEXT_2_POINT 2

typedef struct point Point;
struct point {
  double x;
  double y;
};

// Returns a new Point (note, this is not a pointer to a Point)
// Since it's not allocated using dynamic memory allocation it does not
// need to be freed
Point new_point(double x, double y);

// Prints (on its own line) the Point in the format "x y"
void print_point(Point p);


// -----------------------below-are-functions-added---------------------------
// performs subtraction on two given vectors
Point vector_subtraction(Point p1, Point p2);

// calculate the cross product between two vectors
double cross_product(Point p1, Point p2);

// determines the sign of cross product, returns NEGATIVE, ZERO or POSITIVE
int determine_signed_area(double cross_product);

#endif
