#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "GenerateNode.h"
#define p 0.8
#define q 0.2

adjmatrix *generate_graph(int n_nodes, int n_clusters){
  int e1 = NLINKS;
  adjmatrix *g = malloc(sizeof(adjmatrix));
  g->n = n_nodes;
  g->mat = malloc(sizeof(bool)*g->n*g->n);
  unsigned long *array_nodes = malloc(sizeof(unsigned long)*g->n);
  for(unsigned long i=0; i<g->n; i++)
    {
      array_nodes[i] = i;
    }
  unsigned long *shuffled_array = fisher_yates(array_nodes,g->n);
  g->edges = malloc(e1*sizeof(edge));

  for(int i=0; i<n_nodes; i++)
    {
      int cluster_1 = i/(n_nodes/n_clusters);
      
      for(int j=i+1; j<n_nodes;j++)
	{
	  int cluster_2 = j/(n_nodes/n_clusters);
	  float ra = (float) (rand()%100)/100;
	  //printf("mon random est : %f\n",ra);
	  
	  if(cluster_1 == cluster_2)
	    {
	      //printf("Rentre dans le if du meme cluster\n");
	      if(ra<p)
		{//Création de l'arête entre deux noeuds du même cluster
		  //printf("On rentre dans le if du meme cluster random < p\n");
		  g->edges[g->e].s = i;//shuffled_array[i] ;
		  g->edges[g->e].t = j;
		  if (++(g->e)==e1) {//increase allocated RAM if needed
		    e1+=NLINKS;
		    g->edges=realloc(g->edges,e1*sizeof(edge));
		  }
		  g->e++;		     
		}	
	    }
	  else
	    {
	      printf("Rentre dans le if du cluster different \n");
	      if(ra<q)
		{//liaison avec un noeud d'un cluster avec un cluster different
		  g->edges[g->e].s = i;
		  g->edges[g->e].t = j;
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

int checkLabelPropagationEnds(adjmatrix *g, unsigned long *labels){

  for(unsigned long i=0; i<g->n; i++)
    {
      
      if(labels[i]!=highest_frequency(g,i,labels))
	return 1;
    }
  return 0;
}

unsigned long* label_propagation(adjmatrix *g)
{
  unsigned long *labels = malloc(g->n*sizeof(unsigned long));
  for(int i=0; i<g->n; i++)
    {
      labels[i] = i;
    }
  
  //Randomisation
  //adjmatrix *copy = fisher_yates_adj(g);
  int k = 0;
  while(checkLabelPropagationEnds(g,labels)!=0)
    {
      for(int i=0; i<g->n;i++)
	{
	  unsigned long nd = highest_frequency(g,i,labels);	  	  
	  printf("plus present voisin de %lu : %lu",i,nd);	  
	  labels[i] = nd;	 
	}
      //k++;
    }
  return labels;
}

adjmatrix* fisher_yates_adj(adjmatrix *g)
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
  return g;
}

unsigned long highest_frequency(adjmatrix *g, unsigned long u, unsigned long *labels)
{
  
  unsigned long *array = calloc(g->n,sizeof(unsigned long));
  for(int i=0;i<g->n;i++)
    {
      if(g->mat[u*g->n + i]==1)
	{
	  printf("noeud : %lu labels : %lu\n",i,labels[i]);
	  array[labels[i]]++ ;
	}
    }
  
  unsigned long max = 0;
  unsigned long max_node=0;
  int already_matched = 1;
  int counter_same_node_max = 1;//Lorsque l'on tombe sur un noeud max déjà rencontré on incrémente 0 déja rencontré
  
  for(int i=0;i<g->n;i++)
    {

      if(array[i]==max)
	{
	  if(already_matched==0)
	    {
	      counter_same_node_max++;
	    }
	  else
	    {
	      counter_same_node_max = 1;
	      already_matched = 0;
	    }
	}
      
      if(array[i]>max)
	{
	  //Réinitialisation des valeurs de comptages
	  already_matched = 1;
	  counter_same_node_max = 1;
	  //printf("Rentre dans le if de la recherche du max\n");
	  max = array[labels[i]];
	  max_node = labels[i];	  
	}
    }
  if(already_matched==0)
    return get_random_node(array, max, counter_same_node_max, g->n);
  free(array);
  return max_node;
    
}

unsigned long get_random_node(unsigned long *array, unsigned long max, int counter,int length)
{
  int rd = rand()%counter;
  int k=0;
  for(unsigned long i=0; i<length; i++)
    {
      if(k==rd)
	return i;
      
      if(array[i]==max)
	k++;
    }
  //N'est pas censé renvoyé -1
  return -1;
}

void mergeSort(unsigned long *array, int l, int r)
{
  if(l<r)
    {
      int m = l + (r-l)/2;
      mergeSort(array,l,m);
      mergeSort(array,m+1,r);
      merge(array,l,m,r);
    }
  
}

void merge(unsigned long *array, int debut,int pivot, int fin)
{
  int i,j,k;
  int n1 = pivot - debut + 1;
  int n2 = fin-pivot;
  int *L = calloc(n1,sizeof(int));
  int *R = calloc(n2,sizeof(int));
  for(i=0;i<n1;i++)
    {
      L[i] = array[debut+i];      
    }
  for(j=0; j<n2; j++)
    {
      R[j] = array[pivot+1+j];
    }
  i = 0;
  j = 0;
  k = debut;
  while(i<n1 && j<n2)
    {
      if(L[i]<=R[i])
	{
	  array[k] = R[j];
	  i++;
	}
      else
	{
	  array[k] = L[i];
	  j++;
	}
      k++;
    }
  while(i<n1)
    {
      array[k] = L[i];
      k++;
      i++;
    }
  while(j<n2)
    {
      array[k] = R[j];
      k++;
      j++;
    }
  
  
}
int main(int argc, char **argv)
{
  srand(time(NULL));
  adjmatrix* g;
  time_t t1,t2;

  t1=time(NULL);

  //printf("Reading edgelist from file %s\n",argv[1]);
  g=generate_graph(6,2);

  
  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);

  printf("Building the adjacency matrix\n");
  mkmatrix(g);
  
  for(int i =0; i<g->n;i++)
    {
      printf("noeud %lu:\n",i);
      for(int j =0; j<g->n; j++)
	{
	  if(g->mat[g->n *i +j]==1)
	    printf("nd %u,",j);	  
	}
      printf("\n");
    }
  
  unsigned long *labels = label_propagation(g);
  printf("Affichage tableau\n");

  for(int i=0;i<g->n;i++)
    {
      printf("%lu,",labels[i]);      
    }
    printf("\n____________\n");
  free_adjmatrix(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}
