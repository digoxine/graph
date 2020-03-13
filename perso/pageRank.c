#include "pageRank.h"

vector prod_matrice(vector T, vector weight){
  vector res;
  res.taille = weight.taille;
  res.vec = calloc(weight.taille,sizeof(double));
  for(int i=0; i<res.taille;i++)
    {
      for(int j=0; j<res.taille; j++)
	{

	  if(T.vec[i*res.taille + j]==0 || weight.vec[j]==0)
	    continue;
	  res.vec[i] += T.vec[i*res.taille +j] * weight.vec[j];	  
	}
    }
  return res;
}

  
    
vector transform_transition_matrix(adjlist *g){
  vector res ;
  res.vec = calloc(g->n*g->n, sizeof(double));
  res.taille = g->n*g->n;
  for(unsigned long u=0; u<g->n; u++)
    {
      if(g->cd[u+1]-g->cd[u]==0)
	continue;
      for(int d=g->cd[u];d<g->cd[u+1];d++)
	{
	  unsigned long v = g->adj[d];
	  res.vec[v*g->n + u] = (double) (1.0/(double)(g->cd[u+1]-g->cd[u]));
	  //printf("u = %lu v = %lu degre de u = %lu\n",u,v,g->cd[u+1] -g->cd[u]); 
	}
    }
  return res;
}

void mkadjlist_oriented(adjlist* g){
	unsigned long i,u,v;
	unsigned long *d=calloc(g->n,sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		//d[g->edges[i].t]++;
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned long));
	g->cd[0]=0;
	for (i=0;i<g->n;i++) {
		g->cd[i+1]=g->cd[i]+d[i];
		d[i]=0;
	}

	g->adj=malloc(g->n * g->n*sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;		 
		//g->adj[ g->cd[v] + d[v]++ ]=u;
	}

	free(d);
	//free(g->edges);
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
  /*
  for(unsigned long i = 0; i<g->n; i++)
    {
      printf("%lu: ",i);
      for(unsigned long j = g->cd[i]; j<g->cd[i+1]; j++)
	{
	  printf("%lu, ",g->adj[j]);
	}
      printf("\n");
    }
  */
  
  vector res = transform_transition_matrix(g);
    printf("Affichage tableau\n");
  for(unsigned long i =0; i<g->n ; i++)
    {
      printf("node %lu : \n",i);
      for(unsigned long j =0; j<g->n; j++)
	{
	  double v = res.vec[i*g->n + j];
	  printf("%02lf,",v);
	}
      printf("\n");
    }
  
  /*
  vector weight;
  weight.taille = g->n;
  double tab[11]= {0,1,0,1,0,1,0,1,0,0,0};
  weight.vec = tab;
  */

  /*p
  Verification multiplication matrice
  vector tab1 ;
  double tab[] = {1,2,3,4};
  tab1.vec = tab;
  tab1.taille = 4;
  vector tab2 ;
  double tabP[] = {3,4};
  tab2.vec = tabP;
  tab2.taille = 2;
  vector test = prod_matrice(tab1, tab2);
  printf("resultat multiplication matrice \n");
  for(unsigned long i = 0; i<test.taille; i++)
    {      
      printf("%lf,",test.vec[i]);
    }

  
  
  printf("Affichage tableau\n");
  for(unsigned long i =0; i<g->n ; i++)
    {
      printf("node %lu : \n",i);
      for(unsigned long j =0; j<g->n; j++)
	{
	  double v = res.vec[i*g->n + j];
	  printf("%02lf,",v);
	}
      printf("\n");
    }

  */
  vector res_page = page_rank(1000,g,0);
  printf("Affichage de la matrice stationnaire/////////////////\n");
  for(int i=0; i<res_page.taille;i++)
    {
      printf("%lf,",res_page.vec[i]);
      
    }
  printf("\n");
  
  free_adjlist(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}

vector page_rank(int nb_ite, adjlist *g, float alpha)
{
  vector T = transform_transition_matrix(g);
  /*
  for(unsigned long i =0; i<g->n ; i++)
    {
      printf("node %lu : \n",i);
      for(unsigned long j =0; j<g->n; j++)
	{
	  double v = T.vec[i*g->n + j];
	  printf("%02lf,",v);
	}
      printf("\n");
    }
  */
  vector P;
  P.taille = g->n;
  P.vec = calloc(P.taille,sizeof(double));
  for(int i=0; i<g->n; i++){
    P.vec[i]=(double) (1.0/P.taille);    
  }
  
  for(int i=0; i<nb_ite;i++)
    {
      P = prod_matrice(T,P);
      P = heuristique(alpha,P);
      P = normalize(P);
    }
  free(T.vec);
  return P;
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
  double sum=0 ;
  double temp=0;
  for(int i=0; i<P.taille;i++)
    sum+=P.vec[i];
  for(int i=0; i<P.taille;i++)
    {
      temp = (double) (1.0-sum);
      P.vec[i] += (double) (temp/((double)P.taille));
    }
  return P;
}
