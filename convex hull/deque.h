#ifndef DEQUE_H
#define DEQUE_H

#include "point.h"

typedef struct deque Deque;

typedef struct node Node;
struct deque {
  Node *bottom;
  Node *top;
  int size;
};

struct node {
  Node *to_bottom;
  Point data;
  Node *to_top;
};

// Create a new empty Deque and return a pointer to it
// runs in constant O(1) time
Deque *new_deque();

// Free the memory associated with a Deque
// runs in linear O(n) time
void free_deque(Deque *deque);

// Add a Point to the top of a Deque
// runs in constant O(1) time
void deque_push(Deque *deque, Point data);

// Add a Point to the bottom of a Deque
// runs in constant O(1) time
void deque_insert(Deque *deque, Point data);

// Remove and return the top Point from a Deque
// runs in constant O(1) time
Point deque_pop(Deque *deque);

// Remove and return the bottom Point from a Deque
// runs in constant O(1) time
Point deque_remove(Deque *deque);

// Return the number of Points in a Deque
// runs in constant O(1) time
int deque_size(Deque *deque);

// -----------------------below-are-functions-added---------------------------
// helper function to create a new node
Node *new_node();

// helper function to enqueue the first node into Deque
void enqueue_the_first_point(Deque *deque, Point data);

// helper function to dequeue the last node from Deque
Point dequeue_the_last_point(Deque *deque);

// helper function to initialise the Deque with first three points in polygon
void initialise(Deque *deque, Point *polygon);

// helper function to construct the convex hull
// the algorithm used here is described in part B
int construct(Deque *deque, Point new_point);

#endif
