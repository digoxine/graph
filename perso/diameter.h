#include "adjarray.h"

//Retourne un sommet de la composante connexe la plus grande
unsigned long find_bigger_connected_component(adjlist *g);

//Retourne la taille de la composante connexe du sommet s
int bfs(adjlist *g, unsigned long s, Visited *visit);

//Retourne le diametre de notre 
int diameter(adjlist *g);

//Retourne un noeud aléatoire parmi les noeuds déjà rencontrés dans visit
unsigned long get_random_node(Visited *v);

//Retourne le chemin de taille maximale déterminée avec Djikstra et partant du sommet s
int max_chemin_djikstra(adjlist *g,unsigned long s,Visited *v);


