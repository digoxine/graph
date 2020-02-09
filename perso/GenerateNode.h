#include "adjmatrix.h"

adjmatrix *generate_graph(int n_nodes, int n_clusters);
unsigned long *fisher_yates(unsigned long *array, int length);
unsigned long* label_propagation(adjmatrix *g);
adjmatrix* fisher_yates_adj(adjmatrix *g);
unsigned long highest_frequency(adjmatrix *g, unsigned long u, unsigned long *labels);

//Check if each node have the highest frequency among its neighbours
int checkLabelPropagationEnds(adjmatrix *g, unsigned long *labels);

void mergeSort(unsigned long *array, int l, int r);
void merge(unsigned long *array, int debut,int pivot, int fin);
