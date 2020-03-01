#include "findTriangle.h"

unsigned long max_degree(adjlist *g)
{
  unsigned long max = 0;
  for(int i=0; i<g->e *2; i++)
    {
      unsigned long deg = g->adj[i];
      if(deg>max)
	max = deg;
    }
  return max;
}


void bucketSort(adjlist *g)
{
  
  for(int i=0; i<g->n; i++)
    {
      int debut = g->cd[i];
      int fin = g->cd[i+1]-1;
      mergeSort(g->adj, debut, fin);
    }
  
  //affichage
  
  for(int i=0; i<g->n; i++)
    {
      printf("Sommet %lu :",i);
      for(int j=g->cd[i]; j<g->cd[i+1];j++)
	printf("%lu ",g->adj[j]);
      printf("\n");
    }
  printf("\n");
  
}


void mergeSort(unsigned long *tab, int debut, int fin)
{
  if(debut<fin)
    {
      int m = debut + (fin-debut)/2;

      mergeSort(tab,debut,m);
      
      mergeSort(tab,m+1,fin);
      
      merge(tab,debut,m,fin);
    }
}

void merge(unsigned long* tab, int debut, int pivot, int fin)
{
  int i, j, k;
  int n1 = pivot-debut +1;
  int n2 = fin-pivot;

  unsigned long L[n1], R[n2];

  for(i=0; i<n1; i++)
    L[i] = tab[debut+i];
  for(j=0 ; j<n2; j++)
    R[j] = tab[pivot+1+j];

  i = 0;
  j = 0;
  k = debut;
  while(i<n1 && j<n2)
    {
      if(L[i] <= R[j])
	{
	  tab[k] = L[i];
	  i++;
	}
      else
	{
	  tab[k] = R[j];
	  j++;
	}
      k++;
    }

  while(i<n1)
    {
      tab[k] = L[i];
      i++;
      k++;
    }
  while(j<n2)
    {
      tab[k] = R[j];
      j++;
      k++;
    }
}


int find_triangle(adjlist *g)
{
  //bucketSort(g);
  sort_test(g);
  if(sort_verification(g)==1)
    printf("liste mal triée\n");
  else
    printf("liste bien triée\n");
  int nbTriangles=0;
  for(unsigned long  i=0; i<g->n; i++)
    {
      if(i%100000==0)
	printf("i = %lu\n",i);
      //La boucle max sera de taille degre 
      for(int j=g->cd[i];j<g->cd[i+1];j++)
	{

	  unsigned long v = g->adj[j];
	  //printf("v = %lu\n",v);

	  if(v<=i)//Ainsi nous ne nous intéressons qu'aux noeuds plus grand que le noeud actuel et comme les noeuds sont triés selon l'ordre croissant nous avons des arcs orientés. On ne recompte pas deux fois une arête
	  continue;
	  //printf("passe le premier continue\n");
	  for(int r=g->cd[v];r<g->cd[v+1];r++)
	    {
	      unsigned long w= g->adj[r];	      
	      if(w<=v)
		continue;
	     
	      for(int s=g->cd[w]; s<g->cd[w+1];s++)
		{
		  unsigned long z = g->adj[s];
		  //printf("u=%lu v=%lu w=%lu z=%lu\n",i,v,w,z);
		  if(z==i)
		  {
		     nbTriangles++;
		     //printf("nombre triangles : %lu \n",nbTriangles);
		      //break;
		  }
		}
	    }
	}
    }
  return nbTriangles;
}


double transitivity_ratio(adjlist *g)
{
  printf("before transitivity\n");
  unsigned long nb_pairs = 0;
  unsigned long u,v,w,z;
  for(int i=0; i<g->n; i++)
    {
       u = g->edges[i].s;
       v = g->edges[i].t;
      for(int j=0; j<g->n; j++)
	{
	  w = g->edges[j].s;
	  z = g->edges[j].t;
	  if(z==v && w!=u)
	    nb_pairs++;
	}
    }
  int nbTriangles = find_triangle(g);
  return  ((double) (nbTriangles)/(double) (nb_pairs));
  
}


double clustering_coeff(adjlist *g)
{
  int nbTriangles = find_triangle(g);
  printf("Le nombre de triangles est de %d\n",nbTriangles);
  printf("le nombre de edges = %lu\n",g->e);
  return ((double)(nbTriangles)/(double) (g->e));
  
}

int main(int argc,char** argv){
  srand(time(NULL));
  
  adjlist* g;
  time_t t1,t2;

  t1=time(NULL);

  printf("Reading edgelist from file %s\n",argv[1]);
  g=readedgelist(argv[1]);

  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);

  printf("Building the adjacency list\n");
  mkadjlist(g);
  
  double transitivity_r = transitivity_ratio(g);

  printf("Le transitivity ratio  est de : %f\n",transitivity_r);
  //int nbTriangles = find_triangle(g);
  //printf("Le nombre de triangles est de %d\n",nbTriangles);
  //double res = clustering_coeff(g);
  //printf("clustering coeff %f\n",res);
  free_adjlist(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}

void sort_test(adjlist* g)
{
  for(int i=0; i<g->n;i++)
    {
      for(int j=g->cd[i]; j<g->cd[i+1]; j++)
	{
	  for(int k=j+1;k<g->cd[i+1];k++)
	    if(g->adj[j]>g->adj[k])
	      {
		unsigned long tmp = g->adj[j];
		g->adj[j] = g->adj[k];
		g->adj[k] = tmp;
	      }
	}
    }
}

int sort_verification(adjlist *g){
 for(int i=0; i<g->n;i++)
    {
      for(int j=g->cd[i]; j<g->cd[i+1]; j++)
	{
	  for(int k=j+1;k<g->cd[i+1];k++)
	    if(g->adj[j]>g->adj[k])
	      {
	        return 1;
	    }
	}
    }
 return 0;
}
