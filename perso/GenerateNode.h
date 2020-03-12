#include "adjmatrix.h"

typedef struct {
  unsigned long *tas;
  int taille;
  int pointer;
}Community_heap;
	       

void fisher_yates(unsigned long *array, int length);

unsigned long* label_propagation(adjmatrix *g);

unsigned long highest_frequency(adjmatrix *g, unsigned long u, unsigned long *labels, unsigned long *shuffled_array);

//Check if each node have the highest frequency among its neighbours
int checkLabelPropagationEnds(adjmatrix *g, unsigned long *labels,unsigned long *copy);

unsigned long get_random_node(unsigned long *array, unsigned long max, int counter, int length,unsigned long *labels);

adjmatrix *generate_graph_2(int n_nodes, int n_clusters);


void display_label(unsigned long *label,int length,unsigned long *shuffled_array);


Community_heap *construct_min_heap(adjmatrix *g);

//Nous n'ajouterons jamais aucune communauté nous ne ferons qu'en supprimer donc pas d'ajout


//Lorsqu'une communauté n'aura plus aucun noeud nous la supprimerons
void remove_community(Community_heap *cp, unsigned long community);


//swap deux elements du tas
void swap(Community_heap *cp, unsigned long index1, unsigned long index2);

//recherche une index d'une communauté dans le tas
int research_heap(Community_heap *cp,unsigned long community);

//La propagation de Louvain
int *label_propagation_louvain(adjmatrix *g);

//Renvoie le nombre de noeuds internes à une communauté renvoie 0
//si il n'y en a pas cela veut dire que la communauté doit être supprimé à tester pour la supprimer du tas
unsigned long get_number_intern_nodes(adjmatrix *g, int community, int *nodes_communities);


//Retourne le degré sortant d'un noeud
unsigned long get_out_degree_node(adjmatrix *g, unsigned long node);

//Retourne
unsigned long get_out_degree_community(adjmatrix *g, int *nodes_communities, int community);


//Retourne 1 si la communauté est vide
int check_empty_community(int *nodes_communities, int community, int length);


//display communities
void display_communities(Community_heap *cp);

double compute_quality_3(adjmatrix *g, int *nodes_communities, Community_heap *cp, unsigned long *degres, unsigned long *ms);

unsigned long get_out_degree_community_2(adjmatrix *g, int *nodes_communities,int current_community,unsigned long *degres);

void update_inner_degrees(adjmatrix *g, int *nodes_communities, unsigned long node,int community_1, unsigned long *ms);
void remove_inner_degrees(adjmatrix *g, int *nodes_communities, unsigned long node, int community, unsigned long *ms);

void display_ms(unsigned long *ms, int length);
