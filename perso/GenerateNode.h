#include "adjmatrix.h"

typedef struct {
  unsigned long nb_nodes;
  unsigned long *nodes;  
} community;
	       

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

//unsigned long *jaccard_label_propagation(adjmatrix *g, unsigned long *labels);

//unsigned long intersection(unsigned long *array,unsigned long vi, unsigned long vj);

unsigned long *louvain(adjmatrix *g);


void remove_from_community(community *communities, unsigned long u);

unsigned long max_quality_community(adjmatrix *g, community *communities);
