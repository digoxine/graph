#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "generate_node_adjlist.h"

#define p 0.01
#define q 0.00000



int main(int argc, char **argv)
{
  srand(time(NULL));
  adjlist* g;
  time_t t1,t2;

  t1=time(NULL);
  
  //printf("Reading edgelist from file %s\n",argv[1]);
  //g = readedgelist(argv[1]);
  //mkmatrix(g);
  g=generate_graph(100,4);
  mkadjlist(g);
  //sort(g);
  /*
  for(unsigned long i=0; i<g->n; i++)
    {
      printf("%lu : ",i);
      for(unsigned long j=g->cd[i]; j<g->cd[i+1]; j++)
	{
	  printf("%lu, ", g->adj[j]);
	}
      printf("\n");
      }*/
  int *labels = label_propagation_louvain(g);
  printf("Affichage labels\n");
  for(int i=0; i<g->n; i++)
    printf("%d, ",labels[i]);
  printf("\n Fin affichage tableau\n");
  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);

  printf("Building the adjacency matrix\n");

  printf("Affichage tableau des edges\n");

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
  free_adjlist(g);
  return 0;
}

adjlist *generate_graph(int n_nodes, int n_clusters)
{
  unsigned long e1 = NLINKS;
  adjlist *g = malloc(sizeof(adjlist));
  g->n=n_nodes;
  g->e=0;
  g->edges = malloc(e1*sizeof(edge));
  
  for(unsigned long i=0; i<n_nodes; i++)
    {
      int cluster_1 = (int) ((double) (i)/( ((double)(n_nodes))/ (double) (n_clusters)) );
      
      for(unsigned long j=i+1; j<n_nodes;j++)
	{
	  if(i==j)
	    continue;
	  int cluster_2 = (int) (j/(n_nodes/n_clusters));
	  //printf("i: %d j: %d cluster 1 : %d cluster 2 : %d\n",i,j,cluster_1,cluster_2);
	  double ra = (double) ((rand()%100)/100.0);
	  if(cluster_1 == cluster_2)
	    {
	      if(ra<p)
		{//Création de l'arête entre deux noeuds du même cluster
		  g->edges[g->e].s = i;
		  g->edges[g->e].t = j;
		  if(++(g->e)==e1)
		    {
		      e1+= NLINKS;
		      g->edges = realloc(g->edges,e1*sizeof(edge));
		    }
		}	     
	    }
	  else
	    {
	      if(ra<q)
		{//liaison avec un noeud d'un cluster avec un cluster different
		  g->edges[g->e].s = i;
		  g->edges[g->e].t = j;
		  if(++(g->e)==e1)
		    {
		      e1+= NLINKS;
		      g->edges = realloc(g->edges,e1*sizeof(edge));
		    }
		}
	    }	  
	}
    }
  g->n++;
  //g->edges = realloc(g->edges,g->e*sizeof(edge));
  //sort(g);
  return g;
}



///////////////////////// Opérations sur les tas de communautés ///////////////////////////

Community_heap *construct_min_heap(adjlist *g)
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




int *label_propagation_louvain(adjlist *g)
{
  unsigned long *deg_community = calloc(g->n, sizeof(unsigned long));
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
      degres[i] = g->cd[i+1]-g->cd[i];
      deg_community[i] = degres[i];
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
	  printf("u : %lu\n",u);
	  //printf("previous community : %lu\n");
	  deg_community[previous_community] -= degres[u];
	  //remove from community      
	  //On note -1 quand un noeud u n'a pas de communauté      
	  //nodes_communities[u]=-1;	  
      
	  //Itération sur les communautés
	  for(unsigned long i=0; i<communities_heap->pointer; i++)//
	    {
	      
	      current_community = communities_heap->tas[i];
	      //remove_inner_degrees(g,nodes_communities,u,current_community,ms);
	      //printf("avant check_community \n");
	      if
		(
		  previous_community==current_community
		  &&
		  check_empty_community(nodes_communities, current_community, g->n)==1
		)
		{
		  remove_community(communities_heap,current_community);
		  i--;
		  
		  continue;
		}
	      
	      //remove_inner_degrees(g,nodes_communities,u,previous_community,ms);
	      //Nous insérons le noeud dans la communauté que l'on étudie
	      nodes_communities[u] = current_community;
	      //On met à jour les inner degrees
	      update_inner_degrees(g, nodes_communities,u,current_community,ms);

	      //On met à jour le noombre de degres possedes par la communauté
	      deg_community[current_community]+=degres[u];
	      
	      current_quality = compute_quality_3(g,nodes_communities,communities_heap,degres,ms,deg_community);

	      if(current_quality==max_quality)
		{

		  //printf("cpt same node : %lu\n",cpt_same_max_node);
		  same_node_max[++cpt_same_max_node] = current_community;
		}
	      
	      if(current_quality>max_quality)
		{
		  cpt_same_max_node = 0;
		  same_node_max[cpt_same_max_node] = current_community;
		  best_community = current_community;
		  max_quality = current_quality;

		  //cpt_same_max_node += 2;
		}	      
	      
	      remove_inner_degrees(g,nodes_communities,u,current_community,ms);
	      deg_community[current_community]-=degres[u];
	    }
	  
	  if(cpt_same_max_node>0)
	    {
	      int r = rand()%(cpt_same_max_node+1);
	      //printf("random : %d\n",r);
	      best_community = same_node_max[r];

	    }
	  nodes_communities[u] = best_community;
	  printf("best community : %lu \n",best_community);
	  //On met à jour les inner degrees
	  update_inner_degrees(g,nodes_communities,u,best_community,ms);
	  deg_community[best_community]+=degres[u];
	}
      printf("nombre de communautés : %lu nombre de communautés previous : %lu \n",current_number_communities,previous_number_communities);
      previous_number_communities = current_number_communities;
      current_number_communities = communities_heap->pointer;
    }
  
  free(communities_heap->tas);
  free(communities_heap);
  free(same_node_max);
  free(ms);
  free(deg_community);
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

int check_empty_community_2(int *nodes_communities,int community, int length, unsigned long *deg_community)
{
  if(deg_community[community]==0)
    return 1;
  return 0;
}

void display_communities(Community_heap *cp)
{
  printf("display communities array\n");
  for(int i=0; i<cp->pointer; i++)
    printf("%lu, ",cp->tas[i]);
  printf("\n");
}


//ToDo peut-être qu'il y a une erreur sur le calcul de la modularité


double compute_quality_3(adjlist *g, int *nodes_communities, Community_heap *cp, unsigned long *degres,unsigned long *ms,unsigned long *deg_community)
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
      //printf("nb nd interne %lf\n",temp);
      //printf("temp : %lf\n",temp);
      //temp -= (double) (1/(2*g->e),2 );
      //temp -= (double) (pow((double) (deg_community[current_community]/(2*g->e)),2 ));
       temp -= (double) (pow(get_out_degree_community_2(g,nodes_communities,current_community,degres)/(2*g->e),2 ));
      res+=temp;
    }
  return res;
}

unsigned long get_out_degree_community_2(adjlist *g, int *nodes_communities,int community, unsigned long *degres)
{
  unsigned long nb_out_degrees_community = 0;
  //printf("g->n : %lu, g->e: %lu\n",g->n,g->e);
  for(unsigned long i=0; i<g->n; i++)
    {
      if(nodes_communities[i]==community)
	nb_out_degrees_community+=degres[i];
    }
  return nb_out_degrees_community;
}


void update_inner_degrees(adjlist *g, int *nodes_communities, unsigned long node,int community,unsigned long *ms)
{

  for(unsigned long j = g->cd[node]; j<g->cd[node+1]; j++)
    if(nodes_communities[g->adj[j]]==community)
      ms[community]++;      

}

void remove_inner_degrees(adjlist *g, int *nodes_communities, unsigned long node, int community, unsigned long *ms)
{
  if(ms[community]==0)
    return;
  for(unsigned long j = g->cd[node]; j<g->cd[node+1]; j++)
    if(nodes_communities[g->adj[j]]==community)
      {
	ms[community]--;
	if(ms[community]==0)
	  return;
      }

}

