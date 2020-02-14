#include "pageRank.h"

vector prod_matrice(vector T, vector weight){
  vector res;
  res.taille = weight.taille;
  res.vec = calloc(weight.taille,sizeof(double));
  for(int i=0; i<res.taille;i++)
    {
      for(int j=0; j<res.taille; j++)
	{
	  if(T.vec[i*res.taille + j]==0 || weight.vec[i]==0)
	    continue;
	  res.vec[i] += T.vec[i*res.taille +j] * weight.vec[j];
	  
	}
    }
  return res;
}

  
    
vector transform_transition_matrix(adjlist *g){
  vector res ;
  res.vec = malloc(g->n*g->n* sizeof(double));
  res.taille = g->n*g->n;
  for(unsigned long u=0; u<g->n; u++)
    {
      if(g->cd[u+1]-g->cd[u]==0)
	continue;
      for(int d=g->cd[u];d<g->cd[u+1];d++)
	{
	  //printf("avant le v\n");
	  unsigned long v = g->adj[d];
	  //printf("apres le v\n");
	  printf("u = %lu v = %lu\n",u,v); 
	  
	  res.vec[v*g->n + u] = (double) (1/(g->cd[u+1]-g->cd[u]));
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
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}
	//Non oriente justifie 2. Car tous les voisins de u contiennent v et tous les voisins de v contiennent u
	g->adj=malloc(2*g->e*sizeof(unsigned long));

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
  g= readedgelist(argv[1]);
  
  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);

  printf("Building the adjacency matrix\n");
  mkadjlist_oriented(g);
  vector res = page_rank(1000,g,0.1);
  printf("Affichage de la matrice stationnaire/////////////////\n");
  for(int i=0; i<res.taille;i++)
    {
      printf("%.25lf,",res.vec[i]);
      
    }
  printf("\n");

  /*
  vector res = transform_transition_matrix(g);
  vector weight;
  weight.taille = g->n;
  double tab[11]= {0,1,0,1,0,1,0,1,0,0,0};
  weight.vec = tab;
  printf("Affichage tableau\n");
  for(int i =0; i<g->n ; i++)
    {
      printf("node %lu : \n",i);
      for(int j =0; j<g->n; j++)
	{
	  double v = res.vec[i*g->n + j];
	  printf("%02lf,",v);
	}
      printf("\n");
    }


  printf("Affichage du produit de la matrice/////////////////\n");
  vector newvec = prod_matrice(res,weight);
  for(int i=0; i<newvec.taille;i++)
    {
      printf("%lf,",newvec.vec[i]);
      
    }
  printf("\n");
  */
  free_adjlist(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0;
}

vector page_rank(int nb_ite, adjlist *g, float alpha)
{
  vector T = transform_transition_matrix(g);
  vector P;
  
  P.taille = g->n;
  P.vec = calloc(P.taille,sizeof(double));
  printf("P est de taille %d\n",P.taille);
  printf("1/p.taille = %.15lf\n",(double) 1/1002);
  for(int i=0; i<g->n; i++){
    P.vec[i]=(double) 1/P.taille;
    // printf("%P.vec[%d] = %lf ,",i,P.vec[i]);
  }
  

  /*
   printf("Affichage du poids/////////////////\n");
  for(int i=0; i<P.taille;i++)
    {
      printf("%lf,",P.vec[i]);
      
    }
  printf("\n");
  */
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
      v.vec[i] = (1-alpha) * v.vec[i] + alpha * (1/v.taille);
    }
  return v;
}

vector normalize(vector P)
{
  double sum=0 ;
 
  for(int i=0; i<P.taille;i++)
    sum+=P.vec[i];
  //printf("sum = %lf\n",sum);
  for(int i=0; i<P.taille;i++)
    //P.vec[i] += (double) (1/P.taille) * (1-sum);
    P.vec[i] += (double) (1 - sum)/P.taille;
  return P;
}
