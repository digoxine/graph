#include "kcore.h"

Binary_heap *construct_min_heap(adjlist *g)
{
  Binary_heap *res = malloc(sizeof(Binary_heap));
  res->taille = g->n;
  res->pointer = -1;
  res->tas = calloc(g->n,sizeof(unsigned long));
  res->degres = calloc(g->n, sizeof(int));
 
  for(unsigned long i=0; i<g->n;i++)
    {
     
      int degre = g->cd[i+1] - g->cd[i];
       printf("noeud %lu\n degre %d\n",i,degre);
      insert_node_heap(degre,i,res);
    }
  return res;
}

void insert_node_heap(int degre, unsigned long nd, Binary_heap *hp){
  hp->pointer++;
  hp->tas[hp->pointer] = nd;
  hp->degres[hp->pointer] = degre;

  int index = hp->pointer;
  int parent = (index-1)/2;

  while (index>0 && hp->degres[parent]>hp->degres[index])
    {
      swap(hp,index,parent);

      index = parent;
      parent = (int) ((parent-1)/2);
    }
}

void swap(Binary_heap *hp, int index1, int index2)
{
  int tmp = hp->degres[index1];
  hp->degres[index1] = hp->degres[index2];
  hp->degres[index2] = tmp;
  unsigned long tmp2 = hp->tas[index1];
  hp->tas[index1] = hp->tas[index2];
  hp->tas[index2] = tmp2;
}

unsigned long extract_node_heap(Binary_heap *hp)
{
  unsigned long res = hp->tas[0];
  int index = hp->pointer;
  hp->tas[0] = hp->tas[hp->pointer];
  hp->degres[0] = hp->degres[hp->pointer];
  index = 0;
  hp->pointer--;
  int fg = 2*index +1;
  int fd = fg + 1;
  int mini_fils;
  while (index<hp->pointer && fg<hp->pointer && ((hp->degres[fg]<=hp->degres[index])||(hp->degres[fd]<=hp->degres[index]) ))
    {
      if(fg>hp->pointer)
	return res;
      if (fd>hp->pointer)
	{
	  if(hp->degres[fg]<=hp->degres[index])
	    {
	      mini_fils = fg;
	    }
	}
      else if(hp->degres[fg]<=hp->degres[index] && hp->degres[fd] <= hp->degres[index])
	{
	  if(hp->degres[fg]<=hp->degres[fd])
	    mini_fils = fg;
	  else
	    mini_fils = fd;
	  
	}
      else if(hp->degres[fg]<=hp->degres[index])
	{
	  mini_fils = fg;
	}
      else
	{
	  mini_fils = fd;
	}
      swap(hp,mini_fils,index);
      index = mini_fils;
      fg = 2*mini_fils +1;
      fd = fg+1;
    }
}

unsigned long *core_decomposition(adjlist *g){
  unsigned long *res = calloc(g->n,sizeof(unsigned long));
  int i=g->n;
  int c = 0;
  int *degre = calloc(g->n,sizeof(int));
  unsigned long *node_order = calloc(g->n,sizeof(unsigned long));
  for(int i=0; i<g->n;i++)
    {
      degre[i] = g->cd[i+1]-g->cd[i];
    }
  
  Binary_heap *tas = construct_min_heap(g);
  while(tas->pointer>=0)
    {
      
      unsigned long v=extract_node_heap(tas);
      c = fmax(c,degre[v]);
      update_all_heap(v,g,degre,tas);
      node_order[i]=v;
      i--;
    }
  return node_order;
}

int main(int argc, char **argv)
{
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

  
 
  unsigned long *res = core_decomposition(g);
 printf("//////////////\n");
  printf("apres maj 1\n");


  
  for(int i=0; i<g->n; i++)
    {
      printf("(node:%lu,degre:%d),",res[i]);
    }
  
  free_adjlist(g);

  t2=time(NULL);

  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0; 
}
 
void update_all_heap(unsigned long nd, adjlist *g, int *deg,Binary_heap *hp)
 {
  
  if(deg[(int) nd] == 0)
    return ;
  deg[(int)nd] = 0;
  
  for(int i=g->cd[(int) nd]; i<g->cd[(int) nd+1];i++)
    {
      
      
      unsigned long v = g->adj[i];
      if(deg[v] ==  0)
	continue;
      int ind = research_heap(hp,v);
      deg[i]--;
      update_one_heap(hp,ind);
    }
  
}


 
void update_one_heap(Binary_heap *hp, int index)
{
  hp->degres[index]--;
  if(index==0)
    return ;
  int parent = (index-1)/2;

  while (index>0 && hp->degres[parent]>hp->degres[index])
    {
      swap(hp,index,parent);

      index = parent;
      parent = (int) ((parent-1)/2);
    }
  while(hp->degres[0]==0)
    extract_node_heap(hp);
}


int research_heap(Binary_heap *hp,unsigned long nd)
{
  int res = -1;
  for(int i=0; i<hp->pointer;i++)
    {
      if(hp->tas[i] == nd)
	return i;
    }
  return res;
}
