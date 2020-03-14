#include "pageRank_2.h"


void mkadjlist_oriented(adjlist* g){
  unsigned long i,u,v;
  unsigned long *d=calloc(g->n,sizeof(unsigned long));

  for (i=0;i<g->e;i++) {
    d[g->edges[i].s]++;
    //d[g->edges[i].t]++;
  }

  g->cd=malloc((g->n+1)*sizeof(unsigned long));
  g->cd[0]=0;
  for (i=1;i<g->n+1;i++) {
    g->cd[i]=g->cd[i-1]+d[i-1];
    d[i-1]=0;
  }

  g->adj=calloc(g->e,sizeof(unsigned long));

  for (i=0;i<g->e;i++) {
    u=g->edges[i].s;
    v=g->edges[i].t;
    g->adj[ g->cd[u] + d[u] ]=v;
    d[u]++;
    //g->adj[ g->cd[v] + d[v]++ ]=u;
  }

  free(d);
  //free(g->edges);
}

vector transform_transition_matrix(adjlist *g)
{
  vector res;
  unsigned long v = 0;
  res.taille = g->cd[g->n];
  res.vec = calloc(res.taille,sizeof(double));  
  
  for(unsigned long u=0; u<g->n; u++)
    {
      for(int d = g->cd[u]; d<g->cd[u+1]; d++)
	{
	  //v = g->adj[d];
	  //res.vec[d] += (double)(g->cd[u+1]-g->cd[u]);
	  res.vec[d] += (1.0/(g->cd[u+1]-g->cd[u]));
	}
    }
  return res;
}

vector prod_matrice(adjlist *g,vector T, vector weight)
{
  vector res;
  res.taille = weight.taille;
  res.vec = calloc(res.taille,sizeof(double));
  for(unsigned long u=0; u<res.taille; u++)
    {
      
      for(unsigned long d=g->cd[u]; d<g->cd[u+1];d++)
	{
	  unsigned long v = g->adj[d];
	  if(weight.vec[v]==0)
	    continue;
	  res.vec[v] += weight.vec[u] * T.vec[d] ;
	}      
    }
  return res;
}


vector heuristique(float alpha,vector v)
{
  for(int i=0; i<v.taille; i++)
    {
      v.vec[i] = (1.0-alpha) * v.vec[i] + (alpha * (1.0/v.taille));
    }
  return v;

}

vector normalize(vector P)
{
  double sum_t=0 ;
  double temp=0;
  for(int i=0; i<P.taille;i++)
    sum_t+=P.vec[i];
  for(int i=0; i<P.taille;i++)
    {
      P.vec[i] += (1.0 - sum_t)/(P.taille);
    }
  return P;
}


vector page_rank(int nb_ite, adjlist *g, float alpha)
 {
   vector T = transform_transition_matrix(g);
   vector P;   
   P.vec = calloc(g->n, sizeof(double)); 
   P.taille = g->n; 
   for(int i=0; i<g->n; i++) 
     {
       P.vec[i] = 1.0 / P.taille; 
     } 

   printf("Debut //\n");
   for(int i=0; i<nb_ite;i++) 
     {
       // display_vector(P);
       P = prod_matrice(g,T,P);
       P = heuristique(alpha,P);
       //display_vector(P);
       P = normalize(P);
       //display_vector(P);
     } 
   //display_vector(P);
   free(T.vec); 
   return P; 
 } 

void display_vector(vector v)
{
  for(int i=0 ; i<v.taille; i++)
    {
      printf("%lf, ",v.vec[i]);
    }
  printf("\n");
}


int main(int argc, char **argv) 
{ 
  srand(time(NULL)); 
  adjlist* g; 
  time_t t1,t2; 
  t1=time(NULL); 

  printf("Reading edgelist from file %s\n",argv[1]); 
  g = readedgelist(argv[1]); 

  printf("Number of nodes: %lu\n",g->n); 
  printf("Number of edges: %lu\n",g->e);

  printf("Building the adjacency matrix\n");
  mkadjlist_oriented(g);
  printf("mkadjlist \n");

  vector res_page = page_rank(10,g,0.1);
  display_vector(res_page);
  free(res_page.vec);
  free_adjlist(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}
