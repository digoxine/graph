#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adjarray.h"
typedef struct
{
  int taille;
  double *vec;
} vector;


void mkadjlist_oriented(adjlist* g);
vector transform_transition_matrix(adjlist *g);
vector prod_matrice(adjlist *g,vector T, vector weight);
vector heuristique(float alpha,vector v);
vector normalize(vector P);
vector page_rank(int nb_ite, adjlist *g, float alpha);
void display_vector(vector v);
