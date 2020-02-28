#include "adjarray.h"
//Retourne le nombre de triangle trouvé 
int find_triangle(adjlist *g);

//Tri bucket sort pour notre problème. Modifie g->adj directement. 
void bucketSort(adjlist *g);

//Tri fusion
void mergeSort(unsigned long *tab, int debut, int fin);
//Tri fusion unit
void merge(unsigned long* tab, int debut, int pivot, int fin);

//Donne le degré maximal de la liste de noeuds
unsigned long max_degree(adjlist *g);


void sort_test(adjlist* g);

int sort_verification(adjlist *g);

