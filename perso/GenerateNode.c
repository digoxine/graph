#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "GenerateNode.h"

//#define p 0.2
//#define q 0.001
//compute the maximum of three unsigned long

inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

adjmatrix *generate_graph_2(int n_nodes, int n_clusters, double p, double q)
{
  int e1 = NLINKS;
  adjmatrix *g = malloc(sizeof(adjmatrix));
  g->n = n_nodes;
  g->e = 0;
  g->mat = calloc(g->n*g->n,sizeof(bool));
  g->edges = calloc(e1, sizeof(edge));
  for(unsigned long i=0; i<n_nodes; i++)
    {
      int cluster_1 = (int) ((double) (i)/( ((double)(n_nodes))/ (double) (n_clusters)) );
      
      for(unsigned long j=i+1; j<n_nodes;j++)
	{

	  int cluster_2 = (int) (j/(n_nodes/n_clusters));
	  float ra = (float) ((rand()%100)/100.0);
	  if(cluster_1 == cluster_2)
	    {
	      if(ra<p)
		{//Création de l'arête entre deux noeuds du même cluster
		  g->edges[g->e].s = i;
		  g->edges[g->e].t = j;
		  g->mat[i*g->n + j] = 1;
		  g->mat[j*g->n + i] = 1;
		  g->e++;
		}	     
	    }
	  else
	    {
	      if(ra<q)
		{//liaison avec un noeud d'un cluster avec un cluster different
		  g->edges[g->e].s = i;
		  g->edges[g->e].t = j;
		  g->mat[i*g->n + j] = 1;
		  g->mat[j*g->n + i] = 1;
		  g->e++;		  
		}
	    }	  
	}
    }
  return g;
}

	  

void fisher_yates(unsigned long *array, int length)
{
  for(int i=1; i<length;i++)
    {
      int r = (int) (rand()%i);
      unsigned long tmp = array[r];
      array[r] = array[i];
      array[i] = tmp;
    }
}

int checkLabelPropagationEnds(adjmatrix *g, unsigned long *labels, unsigned long *copy){

  for(unsigned long i=0; i<g->n; i++)
    {
      
      if(labels[copy[i]]!=highest_frequency(g,copy[i],copy,labels))
	return 1;
    }
  return 0;
}

unsigned long* label_propagation(adjmatrix *g)
{
  unsigned long *labels = calloc(g->n,sizeof(unsigned long));
  unsigned long *copy = calloc(g->n, sizeof(unsigned long));

  //Initialisation
  for(unsigned long i=0; i<g->n; i++)
    {
      labels[(int)(i)] = i;
      copy[i] = i;
    }  

  while(checkLabelPropagationEnds(g,labels,copy)!=0)
    {
      fisher_yates(copy,g->n);
      for(unsigned long i=0; i<g->n;i++)
	{
	  unsigned long actual_node = copy[i];	  
	  unsigned long nd = highest_frequency(g,actual_node,copy,labels);
	  labels[actual_node] = nd;
	}      
    }
  free(copy);
  return labels;
}


unsigned long highest_frequency(adjmatrix *g, unsigned long u,unsigned long *shuffled_array, unsigned long *labels)
{

  unsigned long *array = calloc(g->n,sizeof(unsigned long));

  for(unsigned long i=0;i<g->n;i++)
    {     
      if(g->mat[u*g->n+i]==1)
	{
	  array[labels[i]]++ ;
	}
    }


  unsigned long max = 0;
  unsigned long max_node=0;
  int already_matched = 1;
  int counter_same_node_max = 1;//Lorsque l'on tombe sur un noeud max déjà rencontré on incrémente 0 déja rencontré

  for(unsigned long i=0;i<g->n;i++)
    {      
      if(array[labels[i]]==max)
	{
	  if(already_matched==0)
	    {
	      counter_same_node_max++;
	    }
	  else
	    {
	      counter_same_node_max = 2;
	      already_matched = 0;
	    }
	}
      
      if(array[labels[i]]>max)
	{
	  //Réinitialisation des valeurs de comptages
	  already_matched = 1;
	  counter_same_node_max = 1;
	  max =  array[i];
	  max_node = i;
	  
	}      
    }
  
  if(already_matched==0 && counter_same_node_max>1)
    {     
      unsigned long rd = rand()%(counter_same_node_max);
      unsigned long k=0;
      for(unsigned long i=0; i<g->n; i++)
	{	  
	  if(array[i]==max)
	    {
	      if(k==rd)
		{
		  max_node = i;
		  break;
		}
	      k++;
	    }
	}
      
    }
  free(array);
  return max_node;
    
}

unsigned long get_random_node(unsigned long *array, unsigned long max, int counter,int length,unsigned long *labels)
{
  unsigned long rd = rand()%(counter);
  unsigned long k=0;
  for(unsigned long i=0; i<length; i++)
    {
      if(k==rd)
	return i;
      
      if(array[labels[i]]==max)
	k++;
    }
  //N'est pas censé renvoyé -1
  return -1;
}


void display_label(unsigned long *label, int length, unsigned long *shuffled_array)
{
  printf("Affichage tableau labels \n");
  for(int i=0; i<length; i++)
    {
      printf("%lu, ",label[i]);
    }
  printf("\n");
}











//////////////////////////// Implémentation de la label propagation Louvain //////////////////////////






///////////////////////// Opérations sur les tas de communautés ///////////////////////////

Community_heap *construct_min_heap(adjmatrix *g)
{
  //Nous créons toutes les communautés Chaque noeud est une communauté au départ
  Community_heap *existing_communities = malloc(sizeof(Community_heap));
  existing_communities->taille = g->n;
  existing_communities->pointer = g->n;
  existing_communities->tas = calloc(g->n,sizeof(unsigned long));
  for(unsigned long i=0; i<existing_communities->taille;i++)
    {
      existing_communities->tas[i] = i;
    }
  return existing_communities;
}


void remove_community(Community_heap *cp, unsigned long community)
{

  int index = research_heap(cp,community);
  if(index<0)
    printf("communauté pas dans le tas\n");

  for(unsigned long i=index; i<cp->pointer-1;i++)
    {
      swap(cp,i,i+1);      
    }
  cp->pointer--;
}

int research_heap(Community_heap *cp,unsigned long community)
{
  for(int i=0; i<cp->pointer;i++)
    {
      if(cp->tas[i] == community)
	return i;
    }
  return -1;
}
  
void swap(Community_heap *cp, unsigned long index1, unsigned long index2)
{
  unsigned long tmp = cp->tas[index1];
  cp->tas[index1] = cp->tas[index2];
  cp->tas[index2] = tmp;
}



///////////////////////// Fin opérations sur les tas de communautés ///////////////////////////




int *label_propagation_louvain(adjmatrix *g)
{

  int *nodes_communities = calloc(g->n,sizeof(int));
  unsigned long *degres = calloc(g->n, sizeof(unsigned long));
  Community_heap *communities_heap = construct_min_heap(g);
  int current_community = -1;
  double current_quality = 0;
  double max_quality = 0;
  int best_community= -1;
  unsigned long previous_number_communities = 0;
  unsigned long current_number_communities = g->n;
  unsigned long cpt_same_max_node = 0;
  unsigned long *same_node_max = calloc(g->n,sizeof(unsigned long));
  unsigned long *ms = calloc(g->n,sizeof(unsigned long));
  //Initialisation
  //nodes_communities    
  for(unsigned long i=0; i<g->n; i++)
    {
      nodes_communities[i] = i;
      degres[i] = get_out_degree_node(g,i);
    }


  while(previous_number_communities!=current_number_communities)
    {

      //iteration sur les noeuds
      for(unsigned long u=0; u<g->n; u++)
	{
	  //Reinitialisation
	  current_quality = 0;
	  max_quality = 0;
	  best_community = -1;
	  int previous_community = nodes_communities[u];
	  remove_inner_degrees(g,nodes_communities,u,previous_community,ms);
	  nodes_communities[u] = -1;
	  //Itération sur les communautés
	  for(unsigned long i=0; i<communities_heap->pointer; i++)//
	    {

	      current_community = communities_heap->tas[i];

	      if(check_empty_community(nodes_communities, current_community, g->n)==1)
		{
		  remove_community(communities_heap,current_community);
		  i--;
		  continue;
		}
	      
	      //Nous insérons le noeud dans la communauté que l'on étudie
	      nodes_communities[u] = current_community;
	      //On met à jour les inner degrees
	      update_inner_degrees(g, nodes_communities,u,current_community,ms);	      
	      current_quality = compute_quality_3(g,nodes_communities,communities_heap,degres,ms);
	      
	      if(current_quality==max_quality)
		{
		  cpt_same_max_node++;		  
		  same_node_max[cpt_same_max_node] = current_community;
		}
	      
	      if(current_quality>max_quality)
		{
		  cpt_same_max_node = 0;
		  same_node_max[cpt_same_max_node] = current_community;
		  best_community = current_community;
		  max_quality = current_quality;
		}	      
	      //On supprime les inner degrees 
	      remove_inner_degrees(g,nodes_communities,u,current_community,ms);
	      nodes_communities[u] = -1;
	    }
	  
	  if(cpt_same_max_node>0)
	    {
	      int r = rand()%(cpt_same_max_node + 1);	      
	      best_community = same_node_max[r];
	    }
	  nodes_communities[u] = best_community;
	  //On met à jour les inner degrees
	  update_inner_degrees(g,nodes_communities,u,best_community,ms);
	}
      //printf("nombre de communautés : %lu nombre de communautés previous : %lu \n",current_number_communities,previous_number_communities);
      previous_number_communities = current_number_communities;
      current_number_communities = communities_heap->pointer;
    }
  
  free(communities_heap->tas);
  free(communities_heap);
  free(same_node_max);
  free(ms);
  return nodes_communities;
  
}


void display_ms(unsigned long *ms, int length)
{
  printf("Affichage du tableau ms \n");
  for(int i=0; i<length; i++)
    printf("%lu, ",ms[i]);
  printf("\nFin tableau\n");
}


int check_empty_community(int *nodes_communities, int community, int length)
{
  for(int i=0; i<length; i++)
    {
      if(nodes_communities[i] == community)
	return 0;
    }
  return 1;
}



void display_communities(Community_heap *cp)
{
  printf("display communities array\n");
  for(int i=0; i<cp->pointer; i++)
    printf("%lu, ",cp->tas[i]);
  printf("\n");
}


//ToDo peut-être qu'il y a une erreur sur le calcul de la modularité





unsigned long get_number_intern_nodes(adjmatrix *g, int community, int *nodes_communities)
{
  //si il retourne 0 il faudra supprimer cette communauté du tas
  unsigned long nb_inter_nodes = 0;
  //On parcourt tous les noeuds et comme ils sont triés on peut se restreindre aux noeuds plus grands.
  //Nous sommes sûr qu'il n'y aura pas d'arêtes comptées deux fois
  for(int i=0; i<g->n; i++)
    {
      for(int j=i+1; j<g->n; j++)
	{
	  if(g->mat[i*g->n + j]==1 && nodes_communities[i]==community && nodes_communities[j]==community)
	    {
	      nb_inter_nodes++;
	    }	    
	}
    }
  return nb_inter_nodes;
}


unsigned long get_out_degree_node(adjmatrix *g, unsigned long node)
{
  unsigned long nb_out_degree = 0;
  for(unsigned long i=node; i<g->n; i++)
    {
      if(g->mat[node*g->n + i] ==1)
	nb_out_degree++;
    }
  return nb_out_degree;
}


unsigned long get_out_degree_community(adjmatrix *g, int *nodes_communities, int community)
{
  unsigned long nb_out_degrees_community = 0;
  for(unsigned long i=0; i<g->n; i++)
    {
      if(nodes_communities[i]==community)
	nb_out_degrees_community+=get_out_degree_node(g,i);
    }
  return nb_out_degrees_community;
}



double compute_quality_3(adjmatrix *g, int *nodes_communities, Community_heap *cp, unsigned long *degres,unsigned long *ms)
{
  double res = 0;
  double temp = 0;
  int current_community = -1;
  for(unsigned long i = 0; i<cp->pointer; i++)
    {
      temp = 0;
      current_community = (int) (cp->tas[i]);
      temp = (double) (ms[current_community]);
      temp /= (double) (g->e);
      //temp -= (double) (pow(degres[current_community]/(2*g->e),2 ));
      temp -= (double) (pow(get_out_degree_community_2(g,nodes_communities,current_community,degres)/(2*g->e),2 ));
      res+=temp;
    }
  return res;
}

unsigned long get_out_degree_community_2(adjmatrix *g, int *nodes_communities,int community, unsigned long *degres)
{
  unsigned long nb_out_degrees_community = 0;
  for(unsigned long i=0; i<g->n; i++)
    {
      if(nodes_communities[i]==community)
	nb_out_degrees_community+=degres[i];
    }
  return nb_out_degrees_community;
}


void update_inner_degrees(adjmatrix *g, int *nodes_communities, unsigned long node,int community,unsigned long *ms)
{
  for(int i=0; i<g->n; i++)
    {
      if(g->mat[node*g->n + i] == 1 && nodes_communities[i] == community)
	ms[community]++;
    }
}

void remove_inner_degrees(adjmatrix *g, int *nodes_communities, unsigned long node, int community, unsigned long *ms)
{
  if(ms[community]==0)
    return;
  for(unsigned long i=0; i<g->n; i++)
    {
      if(g->mat[node*g->n + i] == 1 && nodes_communities[i] == community)
	ms[community]--;
    }
}


int main(int argc, char **argv)
{
  srand(time(NULL));
  adjmatrix* g;
  if(argc<5)
    {
      printf("Error. This program needs 4 arguments:\n \t -The number of nodes you want to create\n \t -The number of clusters you want to create\n \t -The probability for a node to be linked to a node of the same cluster\n \t -The probability for a node to be linked to a node of another cluster\n");
      return 1;
    }
  
  int nb_nodes = atoi(argv[1]);
  int nb_clusters = atoi(argv[2]);
  double p = atof(argv[3]);
  double q = atof(argv[4]);
  //printf("p = %lf q = %lf\n",p,q);
  g=generate_graph_2(nb_nodes,nb_clusters,p,q);
  for(int i=0; i<g->e; i++)
    {
      printf("%lu %lu\n",g->edges[i].s,g->edges[i].t);
    }
  free_adjmatrix(g);

  return 0;
}
