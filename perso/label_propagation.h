#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "adjmatrix.h"

void fisher_yates(unsigned long *array, int length);

unsigned long* label_propagation(adjmatrix *g);

unsigned long highest_frequency(adjmatrix *g, unsigned long u, unsigned long *labels, unsigned long *shuffled_array);

//Check if each node have the highest frequency among its neighbours
int checkLabelPropagationEnds(adjmatrix *g, unsigned long *labels,unsigned long *copy);

void display_label(unsigned long *label,int length,unsigned long *shuffled_array);

//La propagation de Louvain
int *label_propagation_louvain(adjmatrix *g);

//Retourne 1 si la communauté est vide
int check_empty_community(int *nodes_communities, int community, int length);

