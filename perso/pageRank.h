#include <stdio.h>
#include <stdlib.h>
#include "adjarray.h"
typedef struct
{
  int taille;
  double *vec;
} vector;
vector prod_matrice(vector T, vector weight);
vector transform_transition_matrix(adjlist *g);
void mkadjlist_oriented(adjlist* g);
vector page_rank(int t, adjlist *g, float alpha);
vector heuristique(float alpha,vector v);
vector normalize(vector P);
