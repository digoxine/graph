#include "adjarray.h"


typedef struct {
  unsigned long *tas;
  int taille;
  int pointer;
}Community_heap;
	       

adjlist *generate_graph(int n_nodes, int n_clusters);
Community_heap *construct_min_heap(adjlist *g);

//Nous n'ajouterons jamais aucune communauté nous ne ferons qu'en supprimer donc pas d'ajout


//Lorsqu'une communauté n'aura plus aucun noeud nous la supprimerons
void remove_community(Community_heap *cp, unsigned long community);


//swap deux elements du tas
void swap(Community_heap *cp, unsigned long index1, unsigned long index2);

//recherche une index d'une communauté dans le tas
int research_heap(Community_heap *cp,unsigned long community);

//La propagation de Louvain
int *label_propagation_louvain(adjlist *g);



//Retourne
unsigned long get_out_degree_community(adjlist *g, int *nodes_communities, int community);


//Retourne 1 si la communauté est vide
int check_empty_community(int *nodes_communities, int community, int length);


double compute_quality_3(adjlist *g, int *nodes_communities, Community_heap *cp, unsigned long *degres, unsigned long *ms,unsigned long *deg_community);

unsigned long get_out_degree_community_2(adjlist *g, int *nodes_communities,int current_community,unsigned long *degres);

void update_inner_degrees(adjlist *g, int *nodes_communities, unsigned long node,int community_1, unsigned long *ms);
void remove_inner_degrees(adjlist *g, int *nodes_communities, unsigned long node, int community, unsigned long *ms);



int check_empty_community_2(int *nodes_communities,int community, int length, unsigned long *deg_community);
