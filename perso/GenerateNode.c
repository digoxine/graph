#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "GenerateNode.h"

#define p 1
#define q 0.00
//compute the maximum of three unsigned long

inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

adjmatrix *generate_graph_2(int n_nodes, int n_clusters)
{
  int e1 = NLINKS;
  adjmatrix *g = malloc(sizeof(adjmatrix));
  g->n = n_nodes;
  g->e = 0;
  g->mat = calloc(g->n*g->n,sizeof(bool));
  g->edges = calloc(g->n, sizeof(edge));
  for(unsigned long i=0; i<n_nodes; i++)
    {
      int cluster_1 = (int) ((double) (i)/( ((double)(n_nodes))/ (double) (n_clusters)) );
      
      for(unsigned long j=i+1; j<n_nodes;j++)
	{

	  int cluster_2 = (int) (j/(n_nodes/n_clusters));
	  //printf("i: %d j: %d cluster 1 : %d cluster 2 : %d\n",i,j,cluster_1,cluster_2);
	  float ra = (float) ((rand()%100)/100.0);
	  if(cluster_1 == cluster_2)
	    {
	      if(ra<p)
		{//Création de l'arête entre deux noeuds du même cluster
		  g->mat[i*g->n + j] = 1;
		  g->mat[j*g->n + i] = 1;
		  g->e++;
		}
	     
	    }
	  else
	    {
	      if(ra<q)
		{//liaison avec un noeud d'un cluster avec un cluster different
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
  for(unsigned long i=0; i<g->n; i++)
    {
      labels[(int)(i)] = i;
      copy[i] = i;
    }

  
  int k=0;
  while(checkLabelPropagationEnds(g,labels,copy)!=0)
    {
      //if(k==20000)
      //break;
      fisher_yates(copy,g->n);
      for(unsigned long i=0; i<g->n;i++)
	{
	  unsigned long actual_node = copy[i];
	  //printf("Actual node : %lu\n",actual_node);
	  unsigned long nd = highest_frequency(g,actual_node,copy,labels);
	  //copy[i] = nd;
	  labels[actual_node] = nd;
	  //display_label(labels,g->n,copy);
	}
      
      k++;
    }

  free(copy);
  return labels;
}


unsigned long highest_frequency(adjmatrix *g, unsigned long u,unsigned long *shuffled_array, unsigned long *labels)
{

  unsigned long *array = calloc(g->n,sizeof(unsigned long));

  for(unsigned long i=0;i<g->n;i++)
    {
      //printf("i:%d\n",i);
      if(g->mat[u*g->n+i]==1)
	{

	  //printf("noeud : %lu labels : %lu\n",i,labels[i]);
	  //printf("%lu, ",array[labels[i]]);
	  array[labels[i]]++ ;
	}
    }

  //printf("Sortie première boucle\n");
  unsigned long max = 0;
  unsigned long max_node=0;
  int already_matched = 1;
  int counter_same_node_max = 1;//Lorsque l'on tombe sur un noeud max déjà rencontré on incrémente 0 déja rencontré
  // print array
  /*
  printf("Affichage array\n");
  for(unsigned long i=0; i<g->n; i++)
    {
      printf("On est i = %lu et shuffled_array[i] = %lu labels[si] = %lu\n",i,shuffled_array[i],labels[i] );
      printf("%lu,",array[i]);
    }
  printf("\n");
  */

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
	  //printf("Rentre dans le if de la recherche du max\n");
	  max =  array[i];//array[labels[shuffled_array[i]]];
	  max_node = i;
	  
	}
      
    }
  //printf("counter same node max : %lu already match :%d \n",counter_same_node_max,already_matched);
  //printf("max node de boucle : %lu max : %lu\n",max_node,max);
  
  
  if(already_matched==0 && counter_same_node_max>1)
    {
      //printf("Dans le random\n");
      unsigned long rd = rand()%(counter_same_node_max);
      unsigned long k=0;
      //printf("le random : %lu\n",rd);
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
  
  printf("max node : %lu\n",max_node);
  
  free(array);
  return max_node;
    
}

unsigned long get_random_node(unsigned long *array, unsigned long max, int counter,int length,unsigned long *labels)
{
  //printf("le labels : %lu, counter : %d\n",max,counter);
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
  Community_heap *communities_heap = construct_min_heap(g);
  int current_community = -1;
  double current_quality = 0;
  double max_quality = 0;
  int best_community= -1;
  unsigned long nb_internal_nodes = 0;
  unsigned long nb_out_degree = 0;
  //  unsigned long k=0;
  unsigned long previous_number_communities = 0;
  unsigned long current_number_communities = g->n;

  //Initialisation
  //nodes_communities    
  for(unsigned long i=0; i<g->n; i++)
    {
      nodes_communities[i] = i;
    }


  while(previous_number_communities!=current_number_communities)
    {

      //iteration sur les noeuds
      for(unsigned long u=0; u<g->n; u++)
	{
	  //nous n'iterons pas sur ce sur quoi on supprime
	  //copie
	  
	  Community_heap *copy_iterator = malloc(sizeof(Community_heap));
	  copy_iterator->taille = communities_heap->taille;
	  copy_iterator->pointer = communities_heap->pointer;
	  //pas sure que ća marche peut-être doit on malloc avant memcpy
	  copy_iterator->tas = calloc(g->n, sizeof(unsigned long));
	  memcpy(copy_iterator->tas,communities_heap->tas, sizeof(unsigned long) * g->n);

	  //affichage vérification de la maj des communautés
	  /*
	   printf("Affichage tableau communautés après memcpy \n");
	  for(int i=0; i<copy_iterator->pointer; i++)
	  printf("%lu, ",copy_iterator->tas[i]);
	  printf("\nFin affichage tableau communautés copié\n");
	  */

      
	  //Reinitialisation
	  current_quality = 0;
	  max_quality = 0;
	  best_community = -1;


	  int previous_community = nodes_communities[u];
	  //remove from community      
	  //On note -1 quand un noeud u n'a pas de communauté      
	  nodes_communities[u]=-1;
	  
      
	  //Itération sur les communautés
	  for(unsigned long i=0; i<copy_iterator->pointer; i++)
	    {
	      
	      current_community = copy_iterator->tas[i];

	      if(previous_community==current_community && check_empty_community(nodes_communities, current_community, g->n)==1)
		{
		  remove_community(communities_heap,current_community);
		  continue;
		}
	      
	      //printf("current_community : %d\n",current_community);
	      //Nous insérons le noeud dans la communauté que l'on étudie
	      nodes_communities[u] = current_community;
	      
	      //Communauté vide on la supprime et on recommence une itération	  
	      if(check_empty_community(nodes_communities, current_community, g->n)==1)	    
		{
		  printf("la communauté %u est vide \n",current_community);
		  remove_community(communities_heap,current_community);
		  //remove_community(copy_iterator,current_community);
		  //i--;
		  display_communities(communities_heap);
		  continue;
		}

	      current_quality = compute_quality(g,nodes_communities,current_community);	  
	      //debug qualite
	      //printf("qualité : %lf u: %lu community : %d\n",current_quality, u, current_community);
	  
	      //maj qualité

	      if(current_quality>max_quality)
		{
		  best_community = current_community;
		  max_quality = current_quality;		  
		}	      
	      nodes_communities[u] = -1;
	    }
	  
	  nodes_communities[u] = best_community;
	  
	  free(copy_iterator->tas);
	  free(copy_iterator);	  
      
	}
      printf("nombre de communautés : %lu nombre de communautés previous : %lu \n",current_number_communities,previous_number_communities);
      previous_number_communities = current_number_communities;
      current_number_communities = communities_heap->pointer;
      //k++;
      break;
    }

  
  free(communities_heap->tas);
  free(communities_heap);
  return nodes_communities;
  
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



double compute_quality(adjmatrix *g, int *nodes_communities, int community)
{
  double quality = 0;
  double kv = 0;
  double kw = 0;
  double sv = 0;
  double sw = 0;
  for(unsigned long v=0; v<g->n; v++)
    {
      for(unsigned long w=v+1; w<g->n; w++)
	{
	  sv = nodes_communities[v]==community?1.0:-1.0;
	  sw = nodes_communities[w]==community?1.0:-1.0;
	  kv = (double) (get_out_degree_node(g,v));
	  kw = (double) (get_out_degree_node(g,w));
	  quality += (double) ( g->mat[v*g->n+w] - (double) ( (kv*kw)/(g->e *2)) ) * (( (sv*sw) + 1 )/2) ;
	}
    }
  return quality;
}



int main(int argc, char **argv)
{
  srand(time(NULL));
  adjmatrix* g;
  time_t t1,t2;

  t1=time(NULL);
  
  //printf("Reading edgelist from file %s\n",argv[1]);
  //g = readedgelist(argv[1]);
  //mkmatrix(g);
  g=generate_graph_2(12,4);
  

  
  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);

  printf("Building the adjacency matrix\n");

  printf("Affichage tableau des edges\n");
  /*
  for(int i=0; i<g->n; i++)
    {
      for(int j=0; j<g->n; j++)
	{
	  if(g->mat[i*g->n +j]==1)
	    printf("%d %d\n",i,j);
	}
      //printf("\n");
    }
  printf("\n");
  */
  //unsigned long *labels = label_propagation(g);
  int *labels = label_propagation_louvain(g);
  printf("Affichage tableau\n");
  
  for(int i=0;i<g->n;i++)
    {
      printf("%d,",labels[i]);      
    }
    printf("\n____________\n");
  
  free_adjmatrix(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}
