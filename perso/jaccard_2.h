#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "adjarray.h"


double local_jaccard_distance(adjlist *g, unsigned long node1, unsigned long node2);

double direct_influence(adjlist *g, unsigned long u, unsigned long v);


unsigned long intersection_neighbors(adjlist *g, unsigned long node1, unsigned long node2);
unsigned long union_neighbors(adjlist *g, unsigned long node1, unsigned long node2);

//Fonction ensemblistes noeuds-communautés
unsigned long intersection_community(adjlist *g, int *nodes_communities, unsigned long node1, int community);
unsigned long union_community(adjlist *g, int *nodes_communities, unsigned long node1, int community);

//Effet de bord sur ls_ordonnee
void bubble_sort_ls_nodes_with_similarity(double *S_nodes, unsigned long *ls_ordonnee,int length);
//Compute community 
double community_similarity(adjlist *g, int *nodes_communities, unsigned long node1, int community);
double nodes_similarity(adjlist *g, unsigned long node1, unsigned long node2);

double influence(adjlist *g, unsigned long u, unsigned long v, double lambda);

double EI(adjlist *g, unsigned long u, unsigned long v, double lambda);
int *bi_attractor(adjlist *g);
