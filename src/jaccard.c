#include "jaccard.h"


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
unsigned long intersection_neighbors(adjlist *g,unsigned long node1, unsigned long node2)
{
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
	{
	  //printf("lecture : %lu \n",g->adj[l]);
	  l++;
	}
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
  return res;
}


unsigned long union_neighbors(adjlist *g,unsigned long node1, unsigned long node2)
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
  int *visited = malloc(sizeof(int)*g->n);
  for(int i=0;i<g->n;i++)
    visited[i]=-1;
  unsigned long res = 0;
  unsigned long u=0;
  for(unsigned long i=g->cd[node1]; i<g->cd[node1 + 1]; i++)
    {
      u = g->adj[i];
      if(nodes_communities[u]==community)
	{
	  res++;
	}
    }
  free(visited);
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
    return 0;
  res /=(double) union_community(g,nodes_communities,node1,community);
  return res;
}




double nodes_similarity(adjlist *g, unsigned long node1, unsigned long node2)
{
  double inter_res = intersection_neighbors(g,node1,node2);
  double union_res = union_neighbors(g,node1,node2);
  if(inter_res==0 && union_res==0)
    return 1;
  if(inter_res==0)
    return 0;
  return (inter_res/union_res);
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
      max_similarity = S_nodes[u];

      for(unsigned long j=g->cd[u]; j<g->cd[u+1]; j++)
	{
	  temp_similarity = nodes_similarity(g,u,g->adj[j]);
	  max_similarity += temp_similarity;
	  /*if(max_similarity<temp_similarity)
	    {
	      max_similarity = temp_similarity;
	    }
	  */
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
	  counter_same_nodes = 0;
	  max_similarity = -1;
	  unsigned long real_node = ls_ordonnee_nodes[i];
	  //On le retire de sa communauté

	  communities[ nodes_communities[real_node] ] --;
	  nodes_communities[real_node] = -1;
	  
	  for(unsigned long j=0; j<g->n; j++)
	    {
	      if(communities[j]==0)
		continue;
	      //Ajout à la communauté pour calculer la similarité
	      nodes_communities[real_node] = j;
	      communities[ j ]++;

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
	      //On le retire de sa communauté maintenant que l'on a calculé la similarité
	      communities[j]--;
	      nodes_communities[real_node] = -1;
	    }
	  
	  if(counter_same_nodes>0)
	  {
	    int rd = rand() % (counter_same_nodes+1);
	    best_community = same_nodes_array[rd];	    
	  }
	  //On ajoute le noeud à la bonne communauté en n'ommettant pas de mettre à jour le nombre de noeud de la communauté en question	  
	  communities[best_community] ++;
	  nodes_communities[real_node] = best_community;
	}
    }
  free(ls_ordonnee_nodes);
  free(communities);
  free(S_nodes);
  return nodes_communities;
}

void reordering_label(int *labels,int length)
{
  int *lab =malloc(sizeof(int)*length);
  for(int i=0; i<length; i++)
    lab[i]=-1;
  int cpt = -1;
  for(int i=0; i<length; i++)
    {
      if(lab[labels[i]]==-1)
	{
	  cpt++;	 
	  lab[labels[i]] = cpt;
	}    
    }
  for(int i=0; i<length; i++)
    {
      labels[i] = lab[labels[i]];
    }
  free(lab);
}

double classification_score(int *labels,int nb_clusters, int nb_nodes)
{
  int *visited = malloc(nb_nodes*sizeof(int));
  for(int i=0;i<nb_nodes;i++)
    visited[i]=-1;
  int nb_nodes_by_cluster = nb_nodes/nb_clusters;
  double cpt = (double) (nb_nodes);
  for(int i=0; i<nb_clusters; i++)
    {
      for(int j=0; j<nb_nodes_by_cluster;j++)
	{
	  int real_node = i*nb_nodes_by_cluster + j;	 
	  if(visited[labels[real_node]]==-1)
	    {
	      visited[labels[real_node]] = 0;
	      cpt--;
	    }
	}
    }
  return (double) (cpt/nb_nodes);
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  adjlist* g;
  if(argc<2)
    {      
      printf("this program expects the file you want to know the communities clustering\n");
      return 1;
    }
  g = readedgelist(argv[1]);

  mkadjlist(g);
  int *labels =jaccard_label_propagation(g,100);  
  for(int i=0; i<g->n;i++)
    {
      //printf("dans boucle\n");
      printf("%d %d\n",i,labels[i]);
    } 
  //double score = classification_score(labels,4,g->n);
  //printf("score : %lf \n",score);
  free(labels);
  free_adjlist(g);
				      
  return 0;
}
