#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define YES 1
#define NO 0
#define VERTEX_0 0
#define NO_DISTANCE 0
#define NOWHERE -1
#define PROBLEM2A 'a'
#define PROBLEM2B 'b'

/* --- Below are additional definitions and helper functions --- */

typedef struct node Node; // for p2, nodes are simply nodes in graph
struct node{
  int out_degree;
  int *to_nodes;  // array of nodes this node goes to
  int *distances; // array of distances between this node and the others
};

// for p2a, ith element in priority queue stores information for the ith node
// for p2b, element_i_j is stored in distance matrix at D[i][j]
typedef struct element Element;
struct element{
  int from; // the node before this node in the shortest path
  int dist; // shortest distance from vertex_0 to this node
  int been_visited; // YES if visited, NO if not visited yet
};

int read_graph(Node *graph, int size, char problem);              // for p2
Element *create_priority_Q(int size, int lowest_priority);        // for p2a
int next_node(Element *priority_q, Node* graph, int size);        // for p2a
void dijkstra(Node *graph, int graph_size, Element *priority_q);  // for p2a
void print_p2a(Element *priority_q, int size);                    // for p2a

void print_p2b(Element **D, int size, int constraint);            // for p2b
Element **initialise_distance_matrix(int size, int constraint);   // for p2b
void compute_distance_matrix(Element **D, Node *graph, int size,  // for p2b
  int constraint);
/* --- Above are additional definitions and helper functions --- */

/* --- DO NOT CHANGE THE CODE BELOW THIS LINE --- */

void problem_2_a();
void problem_2_b();
void print_usage_and_exit(char **argv);

int main(int argc, char **argv) {
  if (argc != 2) {
    print_usage_and_exit(argv);
  }

  /* If the argument provided is "a" then run problem_2_a(),
   * run problem_2_b() for "b", and fail otherwise. */
  if (strcmp(argv[1], "a") == 0) {
    problem_2_a();
  } else if (strcmp(argv[1], "b") == 0) {
    problem_2_b();
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

/* TODO: Implement your solution to Problem 2.a. in this function. */
void problem_2_a() {
  int size, max_dist; //  size = number of nodes, max_dist = furthest distance
  Node *graph;  // an array of nodes
  Element *priority_q;  // priority_q[i] = min distance to node_i

  // store inputs
  scanf("%d", &size);
  graph = malloc(sizeof(Node) * size);
  assert(graph);
  max_dist = read_graph(graph, size, PROBLEM2A);

  // compute the path
  priority_q = create_priority_Q(size, max_dist);
  dijkstra(graph, size, priority_q);

  print_p2a(priority_q, size);
}

/* TODO: Implement your solution to Problem 2.b. in this function. */
void problem_2_b(){
  int size, constraint; // size = number of nodes, constraint = number of edges
  Node *graph;  // an array of nodes
  Element **D; // D = distance matrix, D[i][j] = min cost to j using i edges

  // store inputs
  scanf("%d", &size);
  graph = malloc(sizeof(Node) * size);
  assert(graph);
  constraint = read_graph(graph, size, PROBLEM2B);

  // compute the path
  D = initialise_distance_matrix(size, constraint);
  compute_distance_matrix(D, graph, size, constraint);

  print_p2b(D, size, constraint);
}

/* ---------------- Below are additional helper functions ---------------- */

// print out the path for p2b
void print_p2b(Element **D, int size, int constraint){
  int *path, edges_stored = 0, node = size - 1, edges_used = constraint;
  int initial_length = 5, curr_length = initial_length;
  // initial_length is randomly chosen, path length will be realloc if needed
  path = malloc(sizeof(int) * initial_length);
  while (node != VERTEX_0){
    // realloc if path length reaches its current max capacity
    // plus 2 because: (1)arrays are zero offset, (2)to store the first node
    if (curr_length == edges_stored + 2){
      curr_length *= 2;
      path = realloc(path, sizeof(int) * curr_length);
    }

    path[edges_stored] = node;  // store node
    node = D[edges_used][node].from; // get last node

    // if there is no path to this node
    if (node == NOWHERE){
      printf("No Path");
      return;
    }
    edges_stored++;
    edges_used--;
  }

  path[edges_stored] = node;
  printf("%d\n", D[constraint][size - 1].dist);  // total distance to V_n-1
  printf("%d\n", edges_stored);
  // print path backward, from V_n-1 to V_0
  for (node = edges_stored; node > -1; node--){
    printf("%d\n", path[node]);
  }
}

// Dynamic programming. Start at using 1 edge, increment the number of edges
// used by 1 for each round and update the table by finding min distance to all
// subsequently reachable nodes using only no more than that amount of edges
// basic idea: D[k][v] = min{ D[k-1][v], D[k-1][u] + dist(u,v) }, (u,v) ∈ E
// again, D = distance matrix, D[i][j] = min distance to node j using i edges
void compute_distance_matrix(Element **D, Node *graph, int size,
  int constraint){

  int edges_used, i_node, i_neighbor, out_degree, new_dist, neighbor;
  int *neighbors, *distances;
  for (edges_used = 0; edges_used < constraint; edges_used++){
    for (i_node = 0; i_node < size; i_node++){
      if (D[edges_used][i_node].been_visited == YES){
        neighbors = graph[i_node].to_nodes;
        out_degree = graph[i_node].out_degree;
        distances = graph[i_node].distances;
        // D[k][v] = min{ D[k-1][v], D[k-1][u] + dist(u,v) }, (u,v) ∈ E
        for (i_neighbor = 0; i_neighbor < out_degree; i_neighbor++){
          neighbor = neighbors[i_neighbor];
          new_dist = D[edges_used][i_node].dist + distances[i_neighbor];

          if (D[edges_used + 1][neighbor].been_visited == NO){
            D[edges_used + 1][neighbor].from = i_node;
            D[edges_used + 1][neighbor].dist = new_dist;
            D[edges_used + 1][neighbor].been_visited = YES;
          }
          else{
            if (D[edges_used + 1][neighbor].dist > new_dist){
              D[edges_used + 1][neighbor].from = i_node;
              D[edges_used + 1][neighbor].dist = new_dist;
            }
          }
        }
      }
    }
  }
}

// distance matrix stores min distances to all nodes using any number of edges
// i_row = number of i edges used, j_column = j_th vertex.
// (constraint + 1)'s below because arrays are zero offset.
Element **initialise_distance_matrix(int size, int constraint){
  int node, edge;
  // D = distance matrix, D[i][j] = min distance to node j using i edges
  Element **D = malloc(sizeof(Element*) * (constraint + 1));

  // for every row
  for (edge = 0; edge < constraint + 1; edge++){
    D[edge] = malloc(sizeof(Element) * size);
    // for every column
    for (node = 0; node < size; node++){
      D[edge][node].from = NOWHERE;
      D[edge][node].been_visited = NO;
    }
  }
  // initialise the matrix with starting point V_0, V_0 is given
  for (edge = 0; edge < constraint + 1; edge++){
    D[edge][VERTEX_0].been_visited = YES;
    D[edge][VERTEX_0].dist = NO_DISTANCE;
  }
  return D;
}

// print out the path for p2a
void print_p2a(Element *priority_q, int size){
  int edges_stored = 0, node = size - 1;
  // initial_length is randomly chosen, path length will be realloc if needed
  int initial_length = 5, curr_length = initial_length;
  int *path = malloc(sizeof(int) * initial_length);

  while (node != VERTEX_0){
    // realloc if path length reaches its current max capacity
    // plus 2 because: (1)arrays are zero offset, (2)to store the first node
    if (curr_length == edges_stored + 2){
      curr_length *= 2;
      path = realloc(path, sizeof(int) * curr_length);
    }

    path[edges_stored] = node;  // store node
    node = priority_q[node].from; // get last node

    // if there is no path to this node
    if (node == NOWHERE){
      printf("No Path");
      return;
    }
    edges_stored++;
  }

  path[edges_stored] = node;
  printf("%d\n", priority_q[size - 1].dist);  // total distance to V_n-1
  printf("%d\n", edges_stored);
  // print path backward, from V_n-1 to V_0
  for (node = edges_stored; node > -1; node--){
    printf("%d\n", path[node]);
  }
}

// dijkstra algorithm implementation
void dijkstra(Node *graph, int graph_size, Element *priority_q){
  int curr_node = VERTEX_0, nodes_covered, *distances, new_dist;
  int i_neighbor, num_neighbor, *neighbors, neighbor;
  for (nodes_covered = 0; nodes_covered < graph_size; nodes_covered++){
    num_neighbor = graph[curr_node].out_degree;
    neighbors = graph[curr_node].to_nodes;
    distances = graph[curr_node].distances;

    for (i_neighbor = 0; i_neighbor < num_neighbor; i_neighbor++){
      neighbor = neighbors[i_neighbor];
      new_dist = distances[i_neighbor] + priority_q[curr_node].dist;

      if (new_dist < priority_q[neighbor].dist){
        priority_q[neighbor].dist = new_dist;
        priority_q[neighbor].from = curr_node;
      }
    }
    priority_q[curr_node].been_visited = YES;

    // find next node (by priority) and early return if exhausted
    curr_node = next_node(priority_q, graph, graph_size);
    if (curr_node == NOWHERE){
      return;
    }
  }
}

// find next unvisited and visitable node with min distance
int next_node(Element *priority_q, Node* graph, int size){
  int node = NOWHERE, curr_dist, index;
  Element *curr_node;
  for (index = 0; index < size; index++){
    curr_node = priority_q + index;

    if (curr_node->been_visited == NO && curr_node->from != NOWHERE){
      // find the first unvisited and visitable node
      if (node == NOWHERE){
        node = index;
        curr_dist = curr_node->dist;
      }
      // if already found a node
      else {
        // update the node if new node has a higher priority (by distance)
        if (curr_dist > curr_node->dist){
          node = index;
          curr_dist = curr_node->dist;
        }
      }
    }

  }
  return node;
}

// initialise priority queue
Element *create_priority_Q(int size, int max_dist){
  int index, lowest_priority = max_dist + 1;
  // plus one to max_dist to create lowest_priority
  Element *priority_q;
  priority_q = malloc(sizeof(Element) * size);

  for (index = 0; index < size; index++){
    priority_q[index].from = NOWHERE;
    priority_q[index].dist = lowest_priority;
    priority_q[index].been_visited = NO;
  }
  priority_q[VERTEX_0].dist = 0;  // VERTEX_0 is given
  return priority_q;
}

// read nodes provided into a graph
int read_graph(Node *graph, int size, char problem){
  int index, out_degree, edge_stored, curr_dist;
  int max_dist = NO_DISTANCE, constraint;

  if (problem == PROBLEM2B){
    scanf("%d", &constraint);
  }

  for (index = 0; index < size; index++){
    scanf("%d", &out_degree);
    graph[index].out_degree = out_degree;
    graph[index].to_nodes = malloc(sizeof(int) * out_degree);
    graph[index].distances = malloc(sizeof(int) * out_degree);
    assert(graph[index].to_nodes);
    assert(graph[index].distances);

    for (edge_stored = 0; edge_stored < out_degree; edge_stored++){
      scanf("%d", graph[index].to_nodes + edge_stored);
      scanf("%d", graph[index].distances + edge_stored);

      if (problem == PROBLEM2A){
        curr_dist = graph[index].distances[edge_stored];
        max_dist = (curr_dist > max_dist ? curr_dist : max_dist);
      }
    }
  }
  return (problem == PROBLEM2A ? max_dist : constraint);
}
