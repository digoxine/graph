#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "GenerateNode.h"

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
