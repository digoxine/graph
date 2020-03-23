#include "kcore.h"


/* 
   ##########################################################################################################
                                              BINARY HEAP
   ##########################################################################################################
*/
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
  while (index<hp->pointer && fg<hp->pointer && ((hp->degres[fg]<hp->degres[index])||(hp->degres[fd]<hp->degres[index]) ))
    {
      if(fg>hp->pointer)
	return res;
      if (fd>hp->pointer)
	{
	  if(hp->degres[fg]<hp->degres[index])
	    {
	      mini_fils = fg;
	    }
	}
      else if(hp->degres[fg]<hp->degres[index] && hp->degres[fd] < hp->degres[index])
	{
	  if(hp->degres[fg]<hp->degres[fd])
	    mini_fils = fg;
	  else
	    mini_fils = fd;
	  
	}
      else if(hp->degres[fg]<hp->degres[index])
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
  return res;
}




void update_all_heap(unsigned long nd, adjlist *g, int *deg,Binary_heap *hp)
 {
  
   deg[(int)nd] = 0;
  
  for(int i=g->cd[nd]; i<g->cd[nd+1];i++)
    {      
      unsigned long v = g->adj[i];
      if(deg[v]==0)
      continue;
      int ind = research_heap(hp,v);
      if(ind<0)
	continue;
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
/*
  #####################################################################################################
                                        Core decomposition
  #####################################################################################################

 */

unsigned long *core_decomposition(adjlist *g,unsigned long *core){
  int i=g->n-1;
  int c = 0;
  int *degre = calloc(g->n,sizeof(int));
  unsigned long *node_order = calloc(g->n,sizeof(unsigned long));

  //INITIALISATION
  for(int i=0; i<g->n;i++)
    {
      degre[i] = g->cd[i+1]-g->cd[i];
    }
  Binary_heap *tas = construct_min_heap(g);
  //FIN INITIALISATION
  
  while(tas->pointer>=0)
    {

      if(tas->degres[0]==0)
	 {
	  node_order[i] = extract_node_heap(tas);
	  i--;
	  continue;
	 }

      unsigned long v=extract_node_heap(tas);
      c = fmax(c,degre[v]);
      core[v] = c;
      update_all_heap(v,g,degre,tas);
      node_order[i]=v;      
      i--;
      
    }
  free(tas->tas);
  free(tas->degres);
  free(tas);
  
  return node_order;
}

/*
  #################################################################################################
                                   Edge density & density_score
  #################################################################################################
 */

double edge_density(adjlist *g)
{
  return (double) (g->e) / ((double) (g->n) * (double) (g->n -1));
}


double *compute_density_score(adjlist *g, unsigned long iteration_max)
{
  double *r = calloc(g->n, sizeof(unsigned long));
  for(int i = 0 ; i<iteration_max; i++)
    {
      for(unsigned long u = 0; u<g->n; u++)
	{
	  for(unsigned long j =g->cd[u]; j<g->cd[u+1]; j++)
	    {
	      if(u>g->adj[j])
		continue;
	      unsigned long v = g->adj[j];
	      if(r[u]<=r[v])
		r[u]++;
	      else
		r[v]++;
	    }
	}
    }
  for(unsigned long i=0; i<g->n; i++)
    r[i]/=iteration_max;
  return r;
}


unsigned long size_densest_core_ordering_prefix(unsigned long *core,int length)
{
  unsigned long max_core = 0;
  unsigned long size = 0 ;

  for(int i=0; i<length; i++)
    {
      if(core[i]==max_core)
	{
	  size++;
	}
      if(core[i]>max_core)
	{
	  size = 0;
	  max_core = core[i];
	}
    }
  return size;
}


int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

unsigned long size_densest_core_ordering_prefix_2(unsigned long *core,int length)
{
  qsort(core,length,sizeof(int),cmpfunc);
  unsigned long max_counter = 0;
  unsigned long counter_current =0;
  unsigned long max_core = 0;
  for(int i=0;i<length;i++)
    {

      if(core[i]==max_core)
	{
	  counter_current ++;
	}
      
      if(core[i]>max_core)
	{
	  max_core=core[i];
	  counter_current=0;
	}
      if(counter_current>max_counter)
	{
	  max_counter=counter_current;
	}
    }
  return max_counter;
}


unsigned long size_densest_core_ordering_prefix_3(unsigned long *core,adjlist *g)
{
  unsigned long  max_core = 0;
  for(int i=0; i<g->n; i++)
    {
      //printf("core[i] = %d \n",core[i]);
      if(max_core<core[i])
	max_core = core[i];
    }
  //printf("max_core : %d \n",max_core);
  double max_prefix=0;
  double current_prefix = 0;
  unsigned long cpt =0;
  for(int i=0; i<max_core; i++)
    {
      cpt =0;
      current_prefix=0;
      for(int j=0; j<g->n; j++)
	{
	  if(core[j]==i)
	    {
	      current_prefix+= (double) (g->cd[j+1]-g->cd[j]);
	      cpt++;
	    }
	}
      if(cpt==0)
	continue;
      current_prefix/=cpt;
      //printf("current_prefix : %lf\n",current_prefix);
      if(current_prefix>max_prefix)
	max_prefix=current_prefix;
    }
  return (int) max_prefix;
}
  

unsigned long max_order(unsigned long *core,int length)
{
  unsigned long res = 0;
  for(int i=0 ; i<length;i++)
    if(res<core[i])
      res=core[i];
  return res;
}

double average_degree_density_3(unsigned long *core, adjlist *g)
{
  unsigned long max_core = max_order(core,g->n);
  double *degres = calloc(max_core+1, sizeof(double));
  double *cpt = calloc(max_core+1,sizeof(double));
  double res = 0;
  unsigned long u = 0;
  unsigned long v = 0;
  for(unsigned long i=0; i<g->e; i++)
    {     
      u = g->edges[i].s;
      v = g->edges[i].t;
      if(core[u]==core[v])
	{
	  degres[core[u]]++; 
	}
    }

  for(int i=0; i<g->n; i++)
    {
      cpt[core[i]]++;
    }  
  for(int i=0; i<max_core+1; i++)
    {
      res+= (double) (degres[i]/cpt[i]);
    }
  
  free(degres);
  free(cpt);
  return (res);
}

double average_degree_density(adjlist *g, unsigned long *order, unsigned long k_first_nodes_order)
{
  /*
    Params ordre de la core decomposition : order
    k_first_nodes_order : p  
   */
  unsigned long u=0;
  unsigned long v=0;
  unsigned long nb_degrees=0;
  int *marked = calloc(g->n,sizeof(int));
  for(unsigned long i=0; i<k_first_nodes_order;i++)
    {
      u = order[i];
      //marked[u]=1;
      for(unsigned long j=g->cd[u]; j<g->cd[u+1];j++)
	{
	  v = g->adj[j];
	  
	  if(order[v]<k_first_nodes_order)
	    {
	      nb_degrees++;
	    }
	  //  marked[v]=1;
	}
    }
  free(marked);
  return (double) ((double) (nb_degrees)/2.0)/((double) (k_first_nodes_order) );
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  
  adjlist* g;
  time_t t1,t2;

  t1=time(NULL);

  //Initialisation 
  g=readedgelist(argv[1]);
  mkadjlist(g);


  unsigned long *core = calloc(g->n, sizeof(unsigned long));
  unsigned long *order = core_decomposition(g,core);

  double avg = average_degree_density(g,order,g->n);
  printf("avarage degree density : %lf\n",avg);

  free_adjlist(g);
  free(order);
  free(core);
  t2=time(NULL);  
  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0; 
}
