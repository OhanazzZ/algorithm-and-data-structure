#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* --- Below are additional definitions and helper functions --- */

typedef struct node Node; // for p3, nodes are nodes in the binary search tree
struct node{
  int data;
  Node *left;
  Node *right;
};

Node *new_node(int num);
void BST_insert(Node **node, Node *new_node, int *size);
Node *read_input_p3(int *size);
int BST_height(Node *node);
int *BST_to_sorted_array(Node *root, int size);
void build_array(Node *node, int *array, int *index);
void BST_balance_insert(Node **node, int *array, int start, int end);
Node *sorted_array_to_balanced_BST(int *sorted_array, int size);
void print_bst(Node *node, int size);
int number_of_lines(int size);
void fill_up_bst(Node **node, int level, int height);

/* --- Above are additional definitions and helper functions --- */

/* --- DO NOT CHANGE THE CODE BELOW THIS LINE --- */

void problem_3();

int main(int argc, char **argv) {
  problem_3();
  return 0;
}

/* --- DO NOT CHANGE THE CODE ABOVE THIS LINE --- */

/* TODO: Implement your solution to Problem 3 in this function. */
void problem_3() {
  int size, *sorted_array = NULL;
  Node *root = read_input_p3(&size), *balanced_bst = NULL;
  printf("%d\n%d\n%d\n", size, BST_height(root), number_of_lines(size));

  sorted_array = BST_to_sorted_array(root, size);
  balanced_bst = sorted_array_to_balanced_BST(sorted_array, size);
  fill_up_bst(&balanced_bst, 1, BST_height(balanced_bst));  //root at level 1
  print_bst(balanced_bst, number_of_lines(size));
}

/* ---------------- Below are additional helper functions ---------------- */

// calculate the number of lines
int number_of_lines(int size){
  int power = 0, result = 2;
  while (size /= 2){
    power++;
  }
  while (power--){
    result *= 2;
  }
  return result - 1;
}

// fill up the tree with -1 wherever required
void fill_up_bst(Node **node, int level, int height){
  if (level != height){
    fill_up_bst(&(*node)->left, level + 1 , height);
    fill_up_bst(&(*node)->right, level + 1 , height);
  }
  if (!(*node)){
    *node = new_node((-1));
  }
}

// print the tree with level order traversal
void print_bst(Node *node, int size){
  Node **queue = malloc(sizeof(Node*) * size);
  queue[0] = node;
  int counter = 1;

  while(counter){
    if (*queue){
      printf("%d\n", (*queue)->data);
      if ((*queue)->left && (*queue)->right){
        queue[counter++] = (*queue)->left;
        queue[counter++] = (*queue)->right;
      }
    }
    counter--;
    queue++;
  }
}

// insert sorted array into a (balanced) binary search tree
Node *sorted_array_to_balanced_BST(int *sorted_array, int size){
  Node *root = NULL;
  BST_balance_insert(&root, sorted_array, 0, size - 1); // array[0, ... size-1]
  return root;
}

// helper function that performs binary insertion
void BST_balance_insert(Node **node, int *array, int start, int end){
  if (start > end){
    return;
  }
  int mid = (start + end) / 2;
  *node = new_node(array[mid]);
  BST_balance_insert(&(*node)->right, array, (mid + 1), end);
  BST_balance_insert(&(*node)->left, array, start, (mid - 1));
}

// convert the binary tree into a sorted array
int *BST_to_sorted_array(Node *root, int size){
  int *array = malloc(sizeof(int)*size), index = 0;
  build_array(root, array, &index);
  return array;
}

// helper function that builds array by performing inorder traversal
void build_array(Node *node, int *array, int *index){
  (node->left ? build_array(node->left, array, index) : NULL);
  array[(*index)++] = node->data;
  (node->right ? build_array(node->right, array, index) : NULL);
}

// find the height of a binary tree
int BST_height(Node *node){
  int left_height, right_height;
  if (!node){
    return 0;
  }
  left_height = BST_height(node->left);
  right_height = BST_height(node->right);
  return (1 + (left_height > right_height ? left_height : right_height));
}

//read p3 input into a binary tree
Node *read_input_p3(int *size){
  int nodes_read, num, input_size;
  Node *root = NULL;

  scanf("%d", &input_size);
  *size = input_size;
  for (nodes_read = 0; nodes_read < input_size; nodes_read++){
    scanf("%d", &num);
    BST_insert(&root, new_node(num), size);
  }
  return root;
}

// helper function that performs binary tree insertion
void BST_insert(Node **node, Node *new_node, int *size){
  if (!(*node)){
    *node = new_node;
    return;
  }
  if (new_node->data == (*node)->data){
    (*size)--;
    return;
  }
  if (new_node->data > (*node)->data){
    BST_insert(&(*node)->right, new_node, size);
  }
  else{
    BST_insert(&(*node)->left, new_node, size);
  }
}

// helper function that creates a new node
Node *new_node(int num){
  Node *node = malloc(sizeof(Node));
  assert(node);
  node->left = NULL;
  node->right = NULL;
  node->data = num;
  return node;
}
