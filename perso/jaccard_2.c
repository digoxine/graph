#include "jaccard_2.h"



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


double community_similarity_2(adjlist *g, int *nodes_communities, unsigned long node1, int community)
{
  double inter_res = (double) (intersection_community(g,nodes_communities,node1,community));
  
 double union_res=(double) union_community(g,nodes_communities,node1,community);
 if(inter_res==0 && union_res==0)
   return 1;
 if(inter_res==0)
   return 0;
 return (inter_res)/(union_res);
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


int *bi_attractor(adjlist *g)
{
  int converging =0;
  double *distances = calloc(g->e,sizeof(double));
  //printf("g->e = %lu\n",g->e);
  //Initialisation
  for(int i=0; i<g->e; i++)
    {
      printf("i = %lu\n",i);
      distances[i] = local_jaccard_distance(g,g->edges[i].s,g->edges[i].t);
      
    }
  while(converging==0)
    {
      for(unsigned long u = 0; u<g->e;u++)
	{
	  printf("u = %lu\n",u);
	  if(distances[u]<1 && distances[u] >0)
	    {
	      distances[u] = distances[u] + direct_influence(g,g->edges[u].s,g->edges[u].t) + EI(g,g->edges[u].s,g->edges[u].t,0.1);
	      converging=0;
	    }
	  if(distances[u]<=0)
	    converging=1;
	  if(distances[u]>=1)
	    converging=1;
	}
    }
  int cpt = 0 ;
  for(int i =0; i<g->e; i++)
    {
      if(distances[i]!=1)
	cpt++;
    }
  int *labels = calloc(2*cpt,sizeof(int));
  cpt=0;
  for(int i=0; i<g->e; i++)
    {
      if(distances[i]!=1)
	{
	  labels[cpt++]=g->edges[i].s;
	  labels[cpt++]=g->edges[i].t;
	}
    }
  printf("Labels \n");
  for(int i=0;i<cpt;i++)
    {
      printf("%d, ",labels[i]);
    }
  printf("\n");
  free(distances);
  return 0;
}


double EI(adjlist *g, unsigned long u, unsigned long v, double lambda)
{
  unsigned long counter_u = 0;
  unsigned long counter_v = 0;
  double res_2 =0.0;
  double res_1 = 0;
  unsigned long nd;
  unsigned long deg_u = g->cd[u+1] - g->cd[u];
  unsigned long deg_v = g->cd[v+1] - g->cd[v];
  double res_inf =0;
  for(unsigned long j=g->cd[u]; j<g->cd[u+1]; j++)
    {
      nd=g->adj[j];
      if(nd==v)
	continue;
      res_inf = influence(g,nd,u,lambda);
      res_1+= (1.0/deg_u) * (sin(1.0-local_jaccard_distance(g,nd,u)) * res_inf);      
    }
  for(unsigned long j=g->cd[v]; j<g->cd[v+1]; j++)
    {
      nd=g->adj[j];
      if(nd==u)
	continue;
      res_inf = influence(g,nd,v,lambda);
      res_2+= (1.0/deg_v) * (sin(1.0-local_jaccard_distance(g,nd,v))*res_inf);      
    }
  return (-1.0 * res_1 - res_2);

}

double influence(adjlist *g,unsigned long u, unsigned long v, double lambda)
{
  double res = 1.0 - local_jaccard_distance(g,u,v);
  if(lambda<=res)
    return res;
  return res-lambda;
}

double local_jaccard_distance(adjlist *g, unsigned long u, unsigned long v)
{
  unsigned long counter_u = 0;
  unsigned long counter_v = 0;
  double res_2 =0.0;
  double res_1 = 0;
  unsigned long nd;
  for(unsigned long j=g->cd[u]; j<g->cd[u+1]; j++)
    {
      nd=g->adj[j];
      if(nd==v)
	continue;
      counter_u++;
      res_2 += (1.0 - nodes_similarity(g,v,nd) );      
    }
  res_2 /=counter_u;

  for(unsigned long j=g->cd[v]; j<g->cd[v+1]; j++)
    {
      nd=g->adj[j];
      if(nd==u)
	continue;
      counter_v++;
      res_1 += (1.0 - nodes_similarity(g,u,nd) );
    }
  res_1/=counter_v;
  return (1/2)*(res_1+res_2);
}

double direct_influence(adjlist *g, unsigned long u, unsigned long v)
{
  double deg_u = (double) (g->cd[u+1]-g->cd[u]);
  double deg_v = (double) (g->cd[v+1]-g->cd[v]);
  double res = sin(1-local_jaccard_distance(g,u,v) );	       
  return -1.0 *( (res/deg_u) + (res/deg_v) );
}

void reordering_label(int *labels,int length)
{
  int current_nd = -1;
  int cpt = -1;
  for(int i=0 ; i<length; i++)
    {
      if(current_nd!=labels[i])
	{
	  current_nd=labels[i];
	  labels[i] = ++cpt;
	}
    }
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  adjlist* g;
 
  //g=generate_graph(n_nodes,nb_clusters);
  //mkadjlist(g)
  if(argc<2)
    {      
      printf("this program expects the file you want to know the communities clustering\n");
      return 1;
    }
  g = readedgelist(argv[1]);
  printf("g->n = %lu\n",g->n);
  mkadjlist(g);
  int *labels =bi_attractor(g);
  free_adjlist(g);
  return 0;
}
