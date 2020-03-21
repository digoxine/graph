#include "louvain.h"


int main(int argc, char **argv)
{
  srand(time(NULL));
  adjlist* g;
  if(argc<2)
    {
      printf("This programs expects the file you want to know the communities\n");
      return 1;
    }
  g = readedgelist(argv[1]);
  mkadjlist(g);
  int *labels = label_propagation_louvain(g);
  for(int i=0; i<g->n; i++)
    printf("%d %d\n",i,labels[i]);
  free_adjlist(g);
  return 0;
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
  int *same_node_max = calloc(g->n,sizeof(unsigned long));
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
	  deg_community[previous_community] -= degres[u];
	  //remove from community      
	  //On note -1 quand un noeud u n'a pas de communauté      
	  nodes_communities[u]=-1;	  
      
	  //Itération sur les communautés
	  for(unsigned long i=0; i<communities_heap->pointer; i++)//
	    {
	      current_community = communities_heap->tas[i];
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
	       //Nous insérons le noeud dans la communauté que l'on étudie
	      nodes_communities[u] = current_community;
	      //On met à jour les inner degrees
	      update_inner_degrees(g, nodes_communities,u,current_community,ms);
	      
	      //remove_inner_degrees(g,nodes_communities,u,previous_community,ms);
	     
	      

	      //On met à jour le noombre de degres possedes par la communauté
	      deg_community[current_community]+=degres[u];
	      
	      current_quality = compute_quality_3(g,nodes_communities,communities_heap,degres,ms,deg_community);

	      if(current_quality==max_quality)
		{
		  same_node_max[++cpt_same_max_node] = current_community;
		}
	      
	      if(current_quality>max_quality)
		{
		  cpt_same_max_node = 0;
		  same_node_max[cpt_same_max_node] = current_community;
		  best_community = current_community;
		  max_quality = current_quality;
		}

	      remove_inner_degrees(g,nodes_communities,u,current_community,ms);
	      deg_community[current_community]-=degres[u];
	      //nodes_communities[u] = -1;
	    }
	  
	  if(cpt_same_max_node>0)
	    {
	      int r = rand()%(cpt_same_max_node+1);
	      best_community = same_node_max[r];

	    }
	  nodes_communities[u] = best_community;
	  //On met à jour les inner degrees
	  update_inner_degrees(g,nodes_communities,u,best_community,ms);
	  deg_community[best_community]+=degres[u];
	}
      
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
      temp -= (double) (pow((double) (deg_community[current_community]/(2*g->e)),2 ));
      // temp -= (double) (pow(get_out_degree_community_2(g,nodes_communities,current_community,degres)/(2*g->e),2 ));
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

