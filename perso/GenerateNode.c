#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "GenerateNode.h"

#define p 1
#define q 0.4
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
      
      if(labels[i]!=highest_frequency(g,i,copy,labels))
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

  //fisher_yates(copy,g->n);
  int k=0;
  while(checkLabelPropagationEnds(g,labels,copy)!=0)
    {
      if(k==1)
	break;
      fisher_yates(copy,g->n);
      for(unsigned long i=0; i<g->n;i++)
	{
	  unsigned long actual_node = copy[i];
	  printf("Actual node : %lu\n",actual_node);
	  unsigned long nd = highest_frequency(g,actual_node,copy,labels);
	  //copy[i] = nd;
	  labels[actual_node] = nd;
	  display_label(labels,g->n,copy);
	}
      
      k++;
    }

  free(copy);
  return labels;
}


unsigned long highest_frequency(adjmatrix *g, unsigned long u,unsigned long *shuffled_array, unsigned long *labels)
{

  unsigned long *array = calloc(g->n,sizeof(unsigned long));
  for(int i=0;i<g->n;i++)
    {
      //printf("i:%d\n",i);
      if(/*labels[i]>=labels[u] && g->mat[labels[(int) (u)]*g->n + i]==1*/g->mat[u*g->n+i]==1 && shuffled_array[i]!=u)
	{
	  // printf("noeud : %lu labels : %lu\n",i,labels[i]);
	  array[labels[i]]++ ;
	}
    }
  //printf("Sortie première boucle\n");
  unsigned long max = 0;
  unsigned long max_node=0;
  int already_matched = 1;
  int counter_same_node_max = 1;//Lorsque l'on tombe sur un noeud max déjà rencontré on incrémente 0 déja rencontré
  // print array

  printf("Affichage array\n");
  for(int i=0; i<g->n; i++)
    {
      printf("%lu,",array[labels[i]]);
    }
  printf("\n");
  

  for(int i=0;i<g->n;i++)
    {

      if(array[labels[shuffled_array[i]]]==max)
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
      
      if(array[labels[shuffled_array[i]]]>max)
	{
	  //Réinitialisation des valeurs de comptages
	  already_matched = 1;
	  counter_same_node_max = 1;
	  //printf("Rentre dans le if de la recherche du max\n");
	  max =  array[labels[shuffled_array[i]]];//array[labels[shuffled_array[i]]];
	  max_node = shuffled_array[i];	  
	}
    }
  /*
  
  if(already_matched==0 && counter_same_node_max>1)
    {
      //printf("Dans le random\n");
      unsigned long rd = rand()%(counter_same_node_max);
      unsigned long k=0;
      for(unsigned long i=0; i<g->n; i++)
	{
	 
	  if(k>=rd)
	    {
	      max_node = labels[i];
	      break;
	    }
      
	  if(array[shuffled_array[labels[i]]]==max)
	    k++;
	}

      }
  */
  printf("max node : %lu\n",max_node);
  free(array);
  return max_node;
    
}

unsigned long get_random_node(unsigned long *array, unsigned long max, int counter,int length,unsigned long *labels)
{
  printf("le labels : %lu, counter : %d\n",max,counter);
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

void fisher_yates_double(unsigned long *labels ,unsigned long *shuffled_array, int length)
{
  int r;
  unsigned long tmp;
  for(int i=1; i<length;i++)
    {
      r = (int) (rand()%i);
      tmp = labels[r];
      labels[r] = labels[i];
      labels[i] = tmp;
      tmp = shuffled_array[r];
      shuffled_array[r] = shuffled_array[i];
      shuffled_array[i] = tmp;      
    }
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

int main(int argc, char **argv)
{
  srand(time(NULL));
  adjmatrix* g;
  time_t t1,t2;

  t1=time(NULL);
  
  printf("Reading edgelist from file %s\n",argv[1]);
  g = readedgelist(argv[1]);
  mkmatrix(g);
  //g=generate_graph_2(6,3);
  

  
  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);

  printf("Building the adjacency matrix\n");

  printf("Affichage tableau des edges\n");
  for(int i=0; i<g->n; i++)
    {
      for(int j=0; j<g->n; j++)
	{
	  if(g->mat[i*g->n +j]==1)
	    printf("%d %d\n",i,j);
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

adjmatrix *generate_graph(int n_nodes, int n_clusters){
  int e1 = NLINKS;
  adjmatrix *g = malloc(sizeof(adjmatrix));
  g->n = 0;
  g->e = 0;
  g->edges = calloc(e1 , sizeof(edge));
  //g->mat = malloc(sizeof(bool)*g->n*g->n);
  //unsigned long *array_nodes = calloc(g->n,sizeof(unsigned long));
  /*
  for(unsigned long i=0; i<g->n; i++)
    {
      array_nodes[i] = i;
      }*/ 
  for(unsigned long i=0; i<n_nodes; i++)
    {
      int cluster_1 = (int) (i/(n_nodes/n_clusters));
      
      for(unsigned long j=i; j<n_nodes;j++)
	{
	  if(i==j)
	    continue;
	  int cluster_2 = (int) ( ((double)(j))/((double)(n_nodes/n_clusters)) );
	  printf("i: %d j: %d cluster 1 : %d cluster 2 : %d\n",i,j,cluster_1,cluster_2);
	  float ra = (float) ((rand()%100)/100.0);
	  if(cluster_1 == cluster_2)
	    {
	      if(ra<p)
		{//Création de l'arête entre deux noeuds du même cluster
		  g->edges[g->e].s = i;
		  g->edges[g->e].t = j;
		  g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
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
		  
		  g->edges[g->e].s = i;
		  g->edges[g->e].t = j;
		  g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		  if (++(g->e)==e1) {//increase allocated RAM if needed
		    e1+=NLINKS;
		    g->edges=realloc(g->edges,e1*sizeof(edge));
		  }
		  g->e++;		  
		}

	    }
	  
	}
    }
  printf("apres boucle\n");
  g->edges=realloc(g->edges,g->e*sizeof(edge));
  g->n++;
  printf("taille %lu \n",g->n);
  printf("affichage tableau edges\n");
  for(int i=0; i<g->n;i++)
    {
      printf("%lu,%lu\n",g->edges[i].s,g->edges[i].t);
    }
  return g;
}
