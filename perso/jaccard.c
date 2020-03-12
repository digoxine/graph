#include "jaccard.h"
#define p 0.3
#define q 0.001


/*
  ###################################################################################################################
                                               Generation graph
  ###################################################################################################################
*/

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
  return g;
}

/*
  ###############################################################################################################
                                        Fonctions ensemblistes 
  ###############################################################################################################
 */





/*
        #################################################################################################
	                                Union intersection noeud-noeud
        #################################################################################################
*/
unsigned long intersection_neighbors(adjlist *g, int *nodes_communities,unsigned long node1, unsigned long node2)
{
  int *visited = malloc(g->n * sizeof(int));  
  unsigned long res = 0;
  unsigned long k=g->cd[node1];
  unsigned long l = g->cd[node2];


  while(k<g->cd[node1+1] && l<g->cd[node2+1])
    {
      if( g->adj[k]  ==  g->adj[l] )
	{
	  res++;
	  k++;
	  l++;
	    }
      if(g->adj[k] > g->adj[l])
	l++;

      if(g->adj[k] < g->adj[l])
	k++;	  
    }
  while(k<g->cd[node1+1])
    {
      if(g->adj[k] == g->adj[l])
	{
	  res++;
	  k++;	
	}
      if(g->adj[k] > g->adj[l])
	break;

      if(g->adj[k] < g->adj[l])
	k++;
    }
  
  while(l<g->cd[node2+1])
    {
      if(g->adj[k] == g->adj[l])
	{
	  res++;
	  l++;	
	}
      if(g->adj[k] < g->adj[l])
	break;

      if(g->adj[k] > g->adj[l])
	l++;
    }
  free(visited);
  return res;
}


unsigned long union_neighbors(adjlist *g, int *nodes_communities,unsigned long node1, unsigned long node2)
{
  unsigned long res = 0;
  int *visited = malloc(sizeof(int)*g->n);
  for(int i=0; i<g->n; i++)
    {
      visited[i] = -1;
    }

  
  for(unsigned long k=g->cd[node1]; k<g->cd[node1+1]; k++)    
    if(visited[  g->adj[k]  ] == -1)
      {
	visited[ g->adj[k] ] = 1;
	res++;
      }
  for(unsigned long l=g->cd[node2]; l<g->cd[node2+1]; l++)
    if(visited[ g->adj[l]  ] == -1)
      {
	visited[ g->adj[l] ] = 1;
	res++;
      }
  
  free(visited);
  return res;
}


/*
        #################################################################################################
	                                Union intersection noeud-communities
        #################################################################################################
 */



unsigned long intersection_community(adjlist *g, int *nodes_communities, unsigned long node1, int community)
{
  unsigned long res = 0;
  unsigned long u=0;
  for(unsigned long i=g->cd[node1]; i<g->cd[node1 + 1]; i++)
    {
      u = g->adj[i];
      if(nodes_communities[u]==community)
	res++;
    }
  return res;
}


unsigned long union_community(adjlist *g, int *nodes_communities, unsigned long node1, int community)
{
  unsigned long res = 0;
  unsigned long u=0;
  int *visited = malloc(g->n*sizeof(int));
  for(int i=0; i<g->n; i++)
    visited[i] = -1;
  for(int i=g->cd[node1]; i<g->cd[node1+1]; i++)
    {
      u = nodes_communities[ g->adj[i] ];
      if(visited[u] == -1)
	res++;
    }
  free(visited);
  return res;
}

/*
               ################################################################
                                          Sort
               ################################################################
*/
void bubble_sort_ls_nodes_with_similarity(double *S_nodes, unsigned long *ls_ordonnee,int length)
{
  double temp =-1;
  unsigned long temp_n = 0;
  for(int i=0; i<length-1; i++)
    {
      for(int j=0; j<length-i-1; j++)
	{
	  if(S_nodes[j] < S_nodes[j+1])
	    {
	      temp = S_nodes[j];
	      S_nodes[j] = S_nodes[j+1];
	      S_nodes[j+1] = temp;
	      
	      temp_n = ls_ordonnee[j];
	      ls_ordonnee[j] = ls_ordonnee[j+1];
	      ls_ordonnee[j+1] = temp_n;
	    }
	}
    }
}

/*
              #####################################################################
                              Compute community similarity
	      #####################################################################		      
*/
double community_similarity(adjlist *g, int *nodes_communities, unsigned long node1, int community)
{

  double res = (double) (intersection_community(g,nodes_communities,node1,community));
  if(res == 0)
    return res;
  res /=(double) union_community(g,nodes_communities,node1,community);
  return res;
}





/*
  ################################################################################################
                                 Jaccard label propagation
  ################################################################################################				 
 */

int *jaccard_label_propagation(adjlist *g, int iteration_max)
{
  //Creation
  int *nodes_communities = calloc(g->n, sizeof(int));
  int iteration = 0;
  double max_similarity = 0;
  double temp_similarity = 0;
  double temp = 0;
  unsigned long *ls_ordonnee_nodes = malloc(sizeof(unsigned long)*g->n);
  double *S_nodes = calloc(g->n, sizeof(unsigned long));
  int *communities = calloc(g->n, sizeof(int));
  int best_community = -1;
  unsigned long counter_same_nodes =0;
  unsigned long *same_nodes_array = calloc(g->n, sizeof(unsigned long ));

  
  //Initialisation
  for(int i=0; i<g->n; i++)
    {
      ls_ordonnee_nodes[i] = (unsigned long) (i); 
      nodes_communities[i] = i;
      communities[i] = 1;
    }
   
  //On calcule les similarites entre noeuds dans le but de creer une liste ordonnée en fonction de celle-ci
  for(unsigned long u=0; u<g->n; u++)
    {
      //printf("u : %u \n",u);
      max_similarity = 0;
      for(unsigned long j=g->cd[u]; j<g->cd[u+1]; j++)
	{
	  temp_similarity = intersection_neighbors(g,nodes_communities,u,g->adj[j]);
	  if(temp_similarity == 0)
	    {
	      continue;
	    }
	  temp_similarity /= union_neighbors(g,nodes_communities, u, g->adj[j]);
	  if(max_similarity<temp_similarity)
	    max_similarity = temp_similarity;
	}
      S_nodes[u] = max_similarity;      
    }
  //Effet de bord ls_ordonnne_nodes est maintenant ordonnee en fonction de la similarite
  //du noeud avec la plus grande similarite vers la moins grande
  bubble_sort_ls_nodes_with_similarity(S_nodes, ls_ordonnee_nodes, g->n);

  for(iteration = 0 ; iteration<iteration_max; iteration++)
    {
	   
      for(unsigned long i=0; i<g->n; i++)
	{
	  //printf("i : %lu \n",i);
	  max_similarity = 0;
	  unsigned long real_node = ls_ordonnee_nodes[i];
	  communities[ nodes_communities[real_node] ] --;
	  nodes_communities[real_node] = -1;
	  for(unsigned long j=0; j<g->n; j++)
	    {
	      if(communities[j]==0)
		continue;		   
	      nodes_communities[real_node] = j;
	      communities[ nodes_communities[real_node] ]++;

	      temp_similarity = community_similarity(g,nodes_communities,real_node,j);
	      if(temp_similarity == max_similarity)
		{
		  same_nodes_array[++counter_same_nodes] = j;
		}
	      else if(temp_similarity>max_similarity)
		{
		  counter_same_nodes = 0 ;
		  same_nodes_array[counter_same_nodes] = j;
		  max_similarity = temp_similarity;
		  best_community = j;
		}
	      communities[ nodes_communities[real_node] ]--;
	      nodes_communities[real_node] = -1;
	    }
	  if(counter_same_nodes>0)
	    {
	      int rd = rand() % (counter_same_nodes+1);
	      best_community = rd;
	    }
	  communities[best_community] ++;
	  nodes_communities[real_node] = best_community;
	}
    }
  free(ls_ordonnee_nodes);
  free(communities);
  free(S_nodes);
  return nodes_communities;
}



int main(int argc, char **argv)
{
  srand(time(NULL));
  adjlist* g;
  time_t t1,t2;

  t1=time(NULL);

  g=generate_graph(5000,6);
  mkadjlist(g);
  int * labels =jaccard_label_propagation(g,2);

  printf("affichage tableau\n");
  for(int i=0; i<g->n; i++)
    {
  printf("%d ",labels[i]);
}
  
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
