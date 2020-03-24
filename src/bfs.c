#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "bfs.h"


//Renvoie la taille de la composante connexe
int bfs(adjlist *g, unsigned long s, Visited *visit){
//Taille de notre composante 
  int tailleComposante=1;
  //marquage de notre sommet
  markNode(visit,s);
  // Construction de notre file first in first out
  Fifo *f = construct_Fifo(g);
  add_fifo(f,s);

  while(isEmpty(f)!=0)
    {
      unsigned long u = pop(f);
      //output
      //printf("Noeud %lu connecté à :\n",u);

      //on passe en revue tous les voisins de u dans g
      for(int i=g->cd[u]; i<g->cd[u+1]; i++){
	unsigned long v = g->adj[i];	
	if(visit->visited[v]==0)
	  {
	    //printf("\t %lu \n",v);
	    add_fifo(f,v);
	    markNode(visit,v);
	    tailleComposante++;
	  }

      }
      //break;
    }
  //printf("Taille de la composante contenant %lu est %d \n",s,tailleComposante);
  return tailleComposante;
}


//Parcours de toutes les composantes connexes
void connectedComponent(adjlist *g){
  //Nous créons notre structure visites
  Visited* visit = malloc(sizeof(Visited));
  visit->taille = g->n;
  visit->nbNoeudsvisites = 0;
  //Taille de la composante maximale
  int cpsTailleMax = 0;
  //On créé notre array de sommets visités
  visit->visited = calloc(g->n , sizeof(unsigned long));
  //Nous allons le garder tout au long de notre recherche pour garder les noeuds non visités qui appartiennent à d'autres composantes connexes
  int nbcpsConnexes = 0;

  while(allVisited(visit)!=0){
    nbcpsConnexes++;
    unsigned long s = (unsigned long) FirstNodeNotVisited(visit);
    int tailleComp = bfs(g,s,visit);
    if(cpsTailleMax<tailleComp)
      {
	cpsTailleMax = tailleComp;
      }
  }

  printf("Il y a exactement %d composantes connexes \n",nbcpsConnexes);
  printf("La composante de taille maximale est %d \n",cpsTailleMax);
}

int main(int argc,char** argv){
	adjlist* g;
	time_t t1,t2;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency list\n");
	mkadjlist(g);


        connectedComponent(g);
	
	free_adjlist(g);

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}
