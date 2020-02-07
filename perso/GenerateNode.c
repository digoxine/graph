#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "GenerateNode.h"
#define p 0.2
#define q 0.01

adjmatrix *generate_graph(int n_nodes, int n_clusters){
  int e1 = NLINKS;
  adjmatrix *g = malloc(sizeof(adjmatrix));
  g->n = n_nodes;
  g->mat = malloc(sizeof(bool)*g->n*g->n);
  unsigned long *array_nodes = malloc(sizeof(unsigned long)*g->n);
  unsigned long *shuffled_array = fisher_yates(array_nodes,g->n);
  g->edges = malloc(e1*sizeof(edge));

  for(int i=0; i<n_nodes; i++)
    {
      int cluster_1 = i/(n_nodes/n_clusters);
      
      for(int j=i; j<n_nodes;j++)
	{
	  int cluster_2 = j/(n_nodes/n_clusters);
	  float ra = (float) (rand()%100)/100;
	  //printf("mon random est : %f\n",ra);
	  
	  if(cluster_1 == cluster_2)
	    {
	      if(ra<p)
		{//Création de l'arête entre deux noeuds du même cluster
		  //printf("On rentre dans le if du meme cluster\n");
		  g->edges[g->e].s = shuffled_array[i] ;
		  g->edges[g->e].t = shuffled_array[j];
		  if (++(g->e)==e1) {//increase allocated RAM if needed
		    e1+=NLINKS;
		    g->edges=realloc(g->edges,e1*sizeof(edge));
		  }
		  g->e++;		     
		}	
	    }
	  else
	    {
	      if(ra<q)
		{//liaison avec un noeud d'un cluster avec un cluster different
		  g->edges[g->e].s = shuffled_array[i];
		  g->edges[g->e].t = shuffled_array[j];
		  if (++(g->e)==e1) {//increase allocated RAM if needed
		    e1+=NLINKS;
		    g->edges=realloc(g->edges,e1*sizeof(edge));
		  }
		  g->e++;
		}	
	    }
	  
	}
    }
  return g;
}
	  

unsigned long *fisher_yates(unsigned long *array, int length)
{
  for(int i=1; i<=length;i++)
    {
      int r = (rand()%i)+1;
      unsigned long tmp = array[r];
      array[r] = array[i];
      array[i] = tmp;
    }
  
  return array;
}

adjmatrix* label_propagation(adjmatrix *g)
{
  for(int i=1;i<=g->n;i++)
    {
      int r = (rand()%i)+1;
      for(int j=0; j<g->n;j++){
	unsigned tmp = g->mat[i*g->n + j];
	g->mat[i*g->n + j] = g->mat[r*g->n + j];
	g->mat[r*g->n +j] = tmp;
 
      }
    }
}

adjmatrix* fisher_yates_adj(adjmatrix *g)
{
  
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  adjmatrix* g;
  time_t t1,t2;

  t1=time(NULL);

  //printf("Reading edgelist from file %s\n",argv[1]);
  g=generate_graph(400,4);

  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);

  printf("Building the adjacency matrix\n");
  mkmatrix(g);
  
  for(int i=0;i<g->n;i++)
    {
      printf("Noeud %d :\n",i);
      for(int j=0; j<g->n; j++)
	{
	   if(g->mat[i * g->n +j]==1)
	    {
	      printf("%d,",j);
	    }
	}
	   printf("\n");
	}
  free_adjmatrix(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}
