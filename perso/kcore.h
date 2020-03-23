#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "adjarray.h"

typedef struct {
  unsigned long *tas;
  int *degres;
  int taille ;
  int pointer;
}Binary_heap;


//Fonctions gestion du tas 
Binary_heap *construct_min_heap(adjlist *tab);
void insert_node_heap(int degre, unsigned long nd, Binary_heap *tab);
unsigned long extract_node_heap(Binary_heap *hp);
void swap(Binary_heap *bp,int index1,int index2);
void update_all_heap(unsigned long nd, adjlist *g, int *deg, Binary_heap *hp);
void update_one_heap(Binary_heap *hp, int index);
int research_heap(Binary_heap *hp,unsigned long nd);

//Effet de bord sur core qui contiendra les noeuds et leur core
unsigned long *core_decomposition(adjlist* g,unsigned long *core);


//degree_density
//double average_degree_density(adjlist *g);

unsigned long size_densest_core_ordering_prefix(unsigned long *core, int length) ;

double edge_density(adjlist *g);


//Compute density
double *compute_density_score(adjlist *g, unsigned long iteration_max);


