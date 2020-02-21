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

Binary_heap *construct_min_heap(adjlist *tab);
void insert_node_heap(int degre, unsigned long nd, Binary_heap *tab);
//void remove
unsigned long extract_node_heap(Binary_heap *hp);
//swap tas et degre index 1 vers index 2
void swap(Binary_heap *bp,int index1,int index2);

unsigned long *core_decomposition(adjlist* g);
void update_all_heap(unsigned long nd, adjlist *g, int *deg, Binary_heap *hp);
void update_one_heap(Binary_heap *hp, int index);
int research_heap(Binary_heap *hp,unsigned long nd);
