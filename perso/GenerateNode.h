#include "adjmatrix.h"

adjmatrix *generate_graph(int n_nodes, int n_clusters);
void fisher_yates(unsigned long *array, int length);
unsigned long* label_propagation(adjmatrix *g);
adjmatrix* fisher_yates_adj(adjmatrix *g);
unsigned long highest_frequency(adjmatrix *g, unsigned long u, unsigned long *labels, unsigned long *shuffled_array);

//Check if each node have the highest frequency among its neighbours
int checkLabelPropagationEnds(adjmatrix *g, unsigned long *labels,unsigned long *copy);

void mergeSort(unsigned long *array, int l, int r);
void merge(unsigned long *array, int debut,int pivot, int fin);
unsigned long get_random_node(unsigned long *array, unsigned long max, int counter, int length,unsigned long *labels);

adjmatrix *generate_graph_2(int n_nodes, int n_clusters);


//void shuffle_matrix(adjlist *g);
void display_label(unsigned long *label,int length,unsigned long *shuffled_array);

void fisher_yates_double(unsigned long *labels,unsigned long *shuffled_array, int length);
