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


int find_triangle(adjlist *g)
{

  sort_test(g);

  int nbTriangles=0;
  for(unsigned long  i=0; i<g->n; i++)
    {
      //La boucle max sera de taille degre 
      for(int j=g->cd[i];j<g->cd[i+1];j++)
	{

	  unsigned long v = g->adj[j];	  
	  if(v<=i)//s'assure que l'on passe qu'une fois
	  continue;
	  for(int r=g->cd[v];r<g->cd[v+1];r++)
	    {
	      unsigned long w= g->adj[r];	      
	      if(w<=v)
		continue;
	     
	      for(int s=g->cd[w]; s<g->cd[w+1];s++)
		{
		  unsigned long z = g->adj[s];		  
		  if(z==i)
		  {
		     nbTriangles++;		     
		  }
		}
	    }
	}
    }
  return nbTriangles;
}


double transitivity_ratio(adjlist *g)
{
  
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
  return ((double)(nbTriangles)/(double) (g->e));
  
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



int main(int argc,char** argv){
  srand(time(NULL));
  
  adjlist* g;
  time_t t1,t2;

  t1=time(NULL);
  //Initialisation
  printf("Reading edgelist from file %s\n",argv[1]);
  g=readedgelist(argv[1]);
  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);
  printf("Building the adjacency list\n");
  mkadjlist(g);



  
  double transitivity_r = transitivity_ratio(g);

  //printf("Le transitivity ratio  est de : %f\n",transitivity_r);

  free_adjlist(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}
