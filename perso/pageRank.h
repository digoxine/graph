#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adjarray.h"
typedef struct
{
  int taille;
  double *vec;
} vector;
vector *prod_matrice(vector *T, vector *weight);
vector transform_transition_matrix(adjlist *g);
void mkadjlist_oriented(adjlist* g);
vector page_rank(int t, adjlist *g, float alpha);
void heuristique(float alpha,vector *v);
void normalize(vector *P);

void transform_transition_matrix_2(adjlist *g, vector *v);
