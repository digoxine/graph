#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "diameter.h"

#define N_DIAMETER 1000


unsigned long find_bigger_connected_component(adjlist *g)
{
  int cpsTailleMax = 0;
  unsigned long nodeMaxComponent=0;
  Visited *visit = malloc(sizeof(Visited));
  visit->taille = g->n;
  visit->nbNoeudsvisites = 0;
  visit->visited = calloc(g->n, sizeof(unsigned long));
  while(allVisited(visit)!=0)
    {
      unsigned long s = (unsigned long) FirstNodeNotVisited(visit);
      int tailleComp = bfs(g,s,visit);
      if(cpsTailleMax<tailleComp)
	{
	  cpsTailleMax = tailleComp;
	  nodeMaxComponent = s;
	}
      
    }
  free(visit->visited);
  free(visit);
  //printf("noeud max = %lu taille = %d",nodeMaxComponent, cpsTailleMax);
  return nodeMaxComponent;
}


int bfs(adjlist *g, unsigned long s, Visited *visit)
{//O(m+nlog(n))
  int tailleComposante=1;
  markNode(visit,s);
  Fifo *f = construct_Fifo(g);
  add_fifo(f,s);

  while(isEmpty(f)!=0)
    {
      unsigned long u = pop(f);

      for(int i=g->cd[u]; i<g->cd[u+1]; i++)
	{
	  unsigned long v = g->adj[i];	
	  if(visit->visited[v]==0)
	    {
	      add_fifo(f,v);
	      markNode(visit,v);
	      tailleComposante++;
	    }	
	}
      //markNode(visit,u);
    }
  free_fifo(f);
  //printf("Taille de la composante contenant %lu est %d \n",s,tailleComposante);
  return tailleComposante;
}


unsigned long get_random_node(Visited *v)
{//O(n)
  int r = rand() % v->nbNoeudsvisites;
  int i = 0;
  for(int j=0; j<v->taille; j++)
    {
      if(v->visited[j]==1)
	{
	  if(r==i)
	    {
	      i=j;
	      break;
	    }
	  i++;
	}      
    }
  //printf("random node %lu\n");
  return (unsigned long) i;
}


int diameter(adjlist *g)
{
  int lowerBoundDiameter=0;
   printf("debut find bigger\n");
  
  unsigned long nodeOfMaxComponent = find_bigger_connected_component(g);
  printf("fin find bigger\n");
  Visited *visit = malloc(sizeof(Visited));
  visit->taille = g->n;
  visit->nbNoeudsvisites = 0;
  visit->visited = calloc(g->n, sizeof(unsigned long));
  

  printf("noeud max component : %lu \n",nodeOfMaxComponent);
  

  int taille = bfs(g,nodeOfMaxComponent,visit);
  printf("taille de notre composante : %d\n",taille);
  //Tous les sommets de la composante connexe contenant s sont à 1 le reste est à 0
  
  //Ici nous allons utiliser une heuristique particulière car si nous avons une composante connexe unique, en essayant de déterminer une lower bound du diamètre en passant en revue tous les noeuds de la composante va majorer bien trop nos temps de calculs .

  //Nous allons donner un nombre spécifique de fois où nous allons partir d'un noeud de notre composante connexe. Ainsi, nous ferons le minimum de tous les diamètre obtenus pour tous les noeuds que nous aurons sélectionnés comme défini précédemment.

  //Il est important de préciser que notre programme ne sera plus déterministe mais probabiliste. Nous espérerons que notre programme donnera une bonne estimation de la lower bound value de notre diamètre
  
  for(int i=0; i<N_DIAMETER;i++)
    {
      unsigned long  u = get_random_node(visit);
      
      int tmp = max_chemin(g,u,visit);
      //visit->visited[u] = 2;
      printf("u:%lu,taille:%d\n",u,tmp);
      if(tmp>lowerBoundDiameter)
	{
	  lowerBoundDiameter = tmp;
	}
      printf("itération %d finie\n",i);
    }
  
  return lowerBoundDiameter;
}
int max_chemin(adjlist *g,unsigned long u, Visited *visit)
{
  int *distances = calloc(g->n, sizeof(int));
  
  Visited *v = malloc(sizeof(Visited));
  v->taille = visit->taille;
  v->nbNoeudsvisites = visit->nbNoeudsvisites;
  v->visited = malloc(v->taille * sizeof(unsigned long));
  
  //Toutes les cases de v->visited a 0 ne font pas partie de la composante connexe
  //Toutes les cases de v->visited font partie de la composante connexe
  
  for(int i=0; i<g->n; i++)
    {
      //Case à 2 ne fait pas partie de la composante connexe
      //case à 0 font partie de la composante connexe mais pas encore visitenn
      if(visit->visited[i]==0)
	v->visited[i]=1;
      if(visit->visited[i]==1)
	v->visited[i]=0;
      // distances[i] = g->n; //la distance ne peut pas etre supérieure au nombre de sommet sachant que chaque arête a un poids de 1.
    }
  //markNode(v,u);
  distances[u]=0;
  Fifo *f = construct_Fifo(g);
  add_fifo(f,u);

  while(isEmpty(f)!=0)
    {
      unsigned long u = pop(f);
      markNode(v,u);
    
      for(int i=g->cd[u]; i<g->cd[u+1];i++)
	{

	  unsigned long neighbor = g->adj[i];
        
	  if(v->visited[neighbor]==0)
	    {
	      add_fifo(f,neighbor);
	      distances[neighbor] = distances[u]+1;
	      markNode(v,neighbor);    
	    }	  
	}           
    }
  int max = 0;
  for(int i=0; i<g->n; i++)
    {
	if(max<distances[i])
	  max=distances[i];
    }
  free_fifo(f);
  free(distances);
  free(v->visited);
  free(v);
  return max;
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
  
  int diam = diameter(g);

  printf("Le diamètre est de %d\n",diam);
  
  free_adjlist(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}

