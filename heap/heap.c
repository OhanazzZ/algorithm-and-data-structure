#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Below are additional definitions and helper functions --- */
#define YES 1
#define NO 0
#define ROOT_INDEX 1
int* read_input_p1();
void print_output_p1(int* heap_array);
void siftdown_1a(int* array, int parent);
void siftdown_1b(int* array, int parent);
void swap(int* n1, int* n2);
/* --- Above are additional definitions and helper functions --- */

/* --- DO NOT CHANGE THE CODE BELOW THIS LINE --- */

void problem_1_a();
void problem_1_b();
void print_usage_and_exit(char **argv);

int main(int argc, char **argv) {
  if (argc != 2) {
    print_usage_and_exit(argv);
  }

  /* If the argument provided is "a" then run problem_1_a(),
   * run problem_1_b() for "b", and fail otherwise. */
  if (strcmp(argv[1], "a") == 0) {
    problem_1_a();
  } else if (strcmp(argv[1], "b") == 0) {
    problem_1_b();
  } else {
    print_usage_and_exit(argv);
  }

  return 0;
}

/* Print the usage information and exit the program. */
void print_usage_and_exit(char **argv) {
  fprintf(stderr, "usage: %s [ab]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* --- DO NOT CHANGE THE CODE ABOVE THIS LINE --- */

/* TODO: Implement your solution to Problem 1.a. in this function. */
void problem_1_a(){
  int index, size, *heap_array = read_input_p1();
  size = heap_array[0];
  // index starts from the rightmost node at the second last level
  for (index = (size + 1) / 2 - 1; index >= ROOT_INDEX; index--){
    siftdown_1a(heap_array, index);
  }
  print_output_p1(heap_array);
}

/* TODO: Implement your solution to Problem 1.b. in this function. */
void problem_1_b() {
  int index, size, *heap_array = read_input_p1();
  size = heap_array[0];
  // index starts from the rightmost node at the second last level
  for (index = (size + 1) / 2 - 1; index >= ROOT_INDEX; index--){
    siftdown_1b(heap_array, index);
  }
  print_output_p1(heap_array);
}

/* ---------------- Below are additional helper functions ---------------- */

// siftdown the heap until either properties are satisfied or it reaches the
// fringe, used in problem 1.a
void siftdown_1b(int* array, int parent){
  // stop siftdown if the fringe is reached, array[0] is the size of heap
  if (parent * 2 <= array[0]){
    int l_child = parent * 2, r_child = l_child + 1;
    // if right-hand max-heap properties not satisfied
    if (!(array[parent]>=array[r_child] && array[r_child]>=array[l_child])){
      int siftdown_right = NO, siftdown_left = NO;

      if (array[parent] < array[l_child]){
          siftdown_left = YES;
          swap(array + parent, array + l_child);
      }

      if (array[parent] < array[r_child]){
          siftdown_right = YES;
          swap(array + parent, array + r_child);
      }

      if (array[l_child] > array[r_child]){
          siftdown_right = YES;
          siftdown_left = YES;
          swap(array + l_child, array + r_child);
      }

      if (siftdown_right){
        siftdown_1b(array, r_child);
      }

      if (siftdown_left){
        siftdown_1b(array, l_child);
      }
      // check properties again after at least one child has been modified
      siftdown_1b(array, parent);
    }
  }
}

// swap two integers
void swap(int* n1, int* n2){
  int temp;
  temp = *n1;
  *n1 = *n2;
  *n2 = temp;
}

// siftdown the heap until either properties are satisfied or it reaches the
// fringe, used in problem 1.b
void siftdown_1a(int* array, int parent){
  int size = array[0];
  int l_child = parent * 2, r_child = l_child + 1, temp, max_child;
  // if properties are violated before function reaches the fringe
  while (parent * 2 <= size &&
    (array[parent] < array[r_child] || array[parent] < array[l_child])){
    // find max between children and swap it with the parent node
    max_child = (array[l_child] > array[r_child] ? l_child : r_child);
    temp = array[max_child];
    array[max_child] = array[parent];
    array[parent] = temp;
    // update indices and siftdown again
    parent = max_child;
    l_child = parent * 2;
    r_child = l_child + 1;
  }
}

// read values into array
int* read_input_p1(){
  int *heap_array, size, index;
  scanf("%d", &size);
  // add one to size because the size is included in the array
  heap_array = malloc(sizeof(int) * (size + 1));
  heap_array[0] = size;
  for (index = 1; index <= size ; index++){
    scanf("%d", heap_array + index);
  }
  return heap_array;
}

// print elements in the heaparray
void print_output_p1(int* heap_array){
  // index starts at 1 as the size is stored in heap_array[0]
  int index = 1, size = heap_array[0];
  for (index = 1; index <= size ; index++){
    printf("%d\n", heap_array[index]);
  }
}
