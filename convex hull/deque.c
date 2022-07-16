#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "deque.h"
#include "point.h"
#include "convex-hull.h"

// Deque is a struct containing a doubly linked list
// Deque and Node are defined in the deque.h

//  - typedef struct deque Deque;
//  - typedef struct node Node;
//  - struct deque {
//  -   Node *bottom;
//  -   Node *top;
//  -   int size;
//  - };
//  - struct node {
//  -   Node *to_bottom;
//  -   Point data;
//  -   Node *to_top;
//  - };
//

// Create a new empty Deque and return a pointer to it
// runs in constant O(1) time
Deque *new_deque() {
  Deque *deque = malloc(sizeof(*deque));
  assert(deque);

  deque->bottom = NULL;
  deque->top = NULL;
  deque->size = 0;

  return deque;
}

// Free the memory associated with a Deque
// runs in linear O(n) time
void free_deque(Deque *deque) {
  assert(deque);
  Node *curr = deque->bottom, *next;

  // free each node
  while (curr){
    next = curr->to_top;
    free(curr);
    curr = next;
  }

  // free the Deque struct itself
  free(deque);
  deque = NULL;
}

// Add a Point to the top of a Deque
// runs in constant O(1) time
void deque_push(Deque *deque, Point data) {
  assert(deque);

  // if Deque was empty
  if (!deque->size){
    enqueue_the_first_point(deque, data);
    return;
  }

  // if Deque was not empty
  // initialise the new node
  Node *node = new_node();
  assert(node);
  node->data = data;
  node->to_top = NULL;
  node->to_bottom = deque->top;

  // replace the old top node
  deque->top->to_top = node;
  deque->top = node;

  // update the size
  deque->size++;
}

// Add a Point to the bottom of a Deque
// runs in constant O(1) time
void deque_insert(Deque *deque, Point data) {
  assert(deque);

  // if Deque was empty
  if (!deque->size){
    enqueue_the_first_point(deque, data);
    return;
  }

  // if Deque was not empty
  // initialise the new node
  Node *node = new_node();
  assert(node);
  node->data = data;
  node->to_bottom = NULL;
  node->to_top = deque->bottom;

  // replace the old bottom node
  deque->bottom->to_bottom = node;
  deque->bottom = node;

  // update the size
  deque->size++;
}

// Remove and return the top Point from a Deque
// runs in constant O(1) time
Point deque_pop(Deque *deque) {
  assert(deque);

  // if this was the last node in Deque
  if (deque->size == 1){
    return dequeue_the_last_point(deque);
  }

  // if this was not the last node in Deque
  // first save the data to return it
  Point data = deque->top->data;
  // replace the top node and free the old top node
  Node *new_top = deque->top->to_bottom;
  new_top->to_top = NULL;
  free(deque->top);
  deque->top = new_top;

  // update the size
  deque->size--;

  return data;
}

// Remove and return the bottom Point from a Deque
// runs in constant O(1) time
Point deque_remove(Deque *deque) {
  assert(deque);

  // if this was the last node in Deque
  if (deque->size == 1){
    return dequeue_the_last_point(deque);
  }

  // if this was not the last node in Deque
  // first save the data to return it
  Point data = deque->bottom->data;

  // replace the bottom node and free the old bottom node
  Node *new_bottom = deque->bottom->to_top;
  new_bottom->to_bottom = NULL;
  free(deque->bottom);
  deque->bottom = new_bottom;

  // update the size
  deque->size--;

  return data;
}

// Return the number of Points in a Deque
// runs in constant O(1) time
int deque_size(Deque *deque) {
  assert(deque);
  return deque->size;
}


// -----------------------below-are-functions-added---------------------------
// helper function to create a new node
Node *new_node(){
  Node *node = malloc(sizeof(*node));
  assert(node);

  return node;
}

// helper function to enqueue the first node into Deque
void enqueue_the_first_point(Deque *deque, Point data){
  assert(deque);
  assert(!deque->size);

  Node *node = new_node();
  assert(node);

  // the first node will be both the top and the bottom of Deque
  node->data = data;
  node->to_top = NULL;
  node->to_bottom = NULL;
  deque->top = node;
  deque->bottom = node;

  // update the size
  deque->size ++;
}

// helper function to dequeue the last node from Deque
Point dequeue_the_last_point(Deque *deque){
  assert(deque);
  assert(deque->size == 1);

  Point data = deque->top->data;

  // the last node will be both the top and the bottom of Deque
  free(deque->top);
  deque->top = NULL;
  deque->bottom = NULL;
  deque->size--;

  return data;
}

// helper function to initialise the Deque with first three points in polygon
void initialise(Deque *deque, Point *polygon){
  Point p0 = polygon[FIRST_POINT];
  Point p1 = polygon[SECOND_POINT];
  Point p2 = polygon[THIRD_POINT];

  // if Left(p0, p1, p2), then
  if (orientation(p0, p1, p2) == LEFT){
    deque_insert(deque, p0);
    deque_push(deque, p1);
    deque_insert(deque, p2);
    deque_push(deque, p2);
  }
  // if Right(p0, p1, p2), then
  else{
    deque_insert(deque, p1);
    deque_push(deque, p0);
    deque_insert(deque, p2);
    deque_push(deque, p2);
  }
}

// helper function to construct the convex hull
// the algorithm used here is described in part B
int construct(Deque *deque, Point new_point){
  // deque looks like <Ct, Ct_1, ..., Cb_1, Cb> , just as defined in Part b
  Point Ct = deque->top->data, Ct_1 = deque->top->to_bottom->data;
  Point Cb = deque->bottom->data, Cb_1 = deque->bottom->to_top->data;
  char orientation_np_bottom = orientation(Cb, Cb_1, new_point);
  char orientation_np_top = orientation(Ct_1, Ct, new_point);

  // if collineart points occur
  if (orientation_np_top == COLLINEAR || orientation_np_bottom == COLLINEAR){
    return INSIDE_HULL_ERROR;
  }

  // if the point is contained in the convex hull formed by points in Deque
  if (orientation_np_top == LEFT && orientation_np_bottom == LEFT){
    return NO_INSIDE_HULL_ERROR;
  }

  // translating pseudo code given in part B into real C code
  Point data;
  while (orientation_np_top == RIGHT){
    data = deque_pop(deque);
    Ct = Ct_1;
    Ct_1 = deque->top->to_bottom->data;
    orientation_np_top = orientation(Ct_1, Ct, new_point);
  }
  deque_push(deque, new_point);

  while (orientation_np_bottom == RIGHT){
    data = deque_remove(deque);
    Cb = Cb_1;
    Cb_1 = deque->bottom->to_top->data;
    orientation_np_bottom = orientation(Cb, Cb_1, new_point);
  }
  deque_insert(deque, new_point);

  return NO_INSIDE_HULL_ERROR;
}
