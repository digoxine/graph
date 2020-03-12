#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "adjarray.h"


//Generation du graph
adjlist *generate_graph(int n_nodes, int n_clusters);

//Utilities

//Fonctions ensemblistes noeuds
unsigned long intersection_neighbors(adjlist *g, int *nodes_communities, unsigned long node1, unsigned long node2);
unsigned long union_neighbors(adjlist *g, int *nodes_communities, unsigned long node1, unsigned long node2);

//Fonction ensemblistes noeuds-communautés
unsigned long intersection_community(adjlist *g, int *nodes_communities, unsigned long node1, int community);
unsigned long union_community(adjlist *g, int *nodes_communities, unsigned long node1, int community);

//Effet de bord sur ls_ordonnee
void bubble_sort_ls_nodes_with_similarity(double *S_nodes, unsigned long *ls_ordonnee,int length);
//Compute community 
double community_similarity(adjlist *g, int *nodes_communities, unsigned long node1, int community);


int *jaccard_label_propagation(adjlist *g, int iteration_max);




