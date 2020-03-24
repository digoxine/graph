#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "adjarray.h"
#include <stdbool.h>
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
unsigned long *core_decomposition(adjlist *g,unsigned long *core, Binary_heap *tas);
bool exists_in_array(unsigned key, unsigned long *array);
//this method is used for searching an elements in array
bool exists_in_array2(unsigned long key, unsigned long*array,unsigned long nb);

// in this method we find the number of edges and number of nodes who are in the subgraphs 
//and calculate the formule = nb_edges*2/nb_nodes
double average_degree_density(unsigned long *array,unsigned long nb,adjlist *g);

// in this method we calculate the number of edges of the densest subgraph and the 
//maximum possible number of edge dans that subgraph
double edge_density2(unsigned long *order, unsigned long nb,adjlist *g);
double edge_density3(unsigned long *order, unsigned long nb,adjlist *g);
double *densest_subgraph(adjlist *g, unsigned long t);
//in this method we searched in first n subgraphs and finding the subgraph with max average_degree_density
//and we returned the number of nodes of that subgraph
 unsigned long core_ordering_prefix_EXO1(unsigned long *order,adjlist* g);
 //like the core_ordering_prefix of first question , here we the same principe but with the new array
 unsigned long core_ordering_prefix_EXO3(unsigned long *nodeScore,adjlist* g);

//this method is for finding the average degree density of whole graph
  double average_degree_density_total(adjlist *g);
  void swap2Long(unsigned long *a, unsigned long *b);
  void swap2Double(double *a, double *b);
  // this method is for sorting the array of nodeScore based on their score
void *sort_node_score(adjlist *g, double *score,unsigned long *nodeScore, unsigned long f);
//in this method we find the degree of nods who are in the order array and their core for exo2
void find_degree_exo2(unsigned long *order,unsigned long *core,adjlist *g);
