#include "adjarray.h"

//Retourne le nombre de triangle trouvé 
int find_triangle(adjlist *g);

//Donne le degré maximal de la liste de noeuds
unsigned long max_degree(adjlist *g);

//Sort
void sort_test(adjlist* g);
int sort_verification(adjlist *g);


double transitivity_ratio(adjlist *g);
double clustering_coeff(adjlist *g);
