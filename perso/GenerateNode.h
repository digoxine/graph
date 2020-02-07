#include "adjmatrix.h"

adjmatrix *generate_graph(int n_nodes, int n_clusters);
unsigned long *fisher_yates(unsigned long *array, int length);
adjmatrix* label_propagation(adjmatrix *g);
adjmatrix* fisher_yates_adj(adjmatrix *g);
