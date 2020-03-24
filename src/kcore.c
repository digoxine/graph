#include "kcore.h"

#define NB_ITER 10 
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
unsigned long *core_decomposition(adjlist *g,unsigned long *core, Binary_heap *tas)
{
  int i=g->n-1;
  int c = 0;
  int *degre = calloc(g->n,sizeof(int));
  unsigned long *node_order = calloc(g->n,sizeof(unsigned long));

  //INITIALISATION
  for(int i=0; i<g->n;i++)
    {
      degre[i] = g->cd[i+1]-g->cd[i];
    }

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

bool exists_in_array(unsigned key, unsigned long *array){
    for(int i=0;i<sizeof(array);i++){
      if(array[i]==key){
        return true;
      }
    }
  return false;

}
//this method is used for searching an elements in array
bool exists_in_array2(unsigned long key, unsigned long*array,unsigned long nb){
    for(int i=0;i<nb;i++){
      if(array[i]==key){
        return true;
      }
    }
  return false;
}

// in this method we find the number of edges and number of nodes who are in the subgraphs 
//and calculate the formule = nb_edges*2/nb_nodes
double average_degree_density(unsigned long *array,unsigned long nb,adjlist *g){
double avg;
unsigned long e=0; // nombre de edges
unsigned long j=0;
unsigned long u,v;
   for(int i=0;i<nb;i++){
     u= array[i];
    for(int k=g->cd[u];k<g->cd[u+1];k++){
       v= g->adj[k];
       if( exists_in_array2(v, array ,sizeof(array))){
        e++;
      } 
    }
}
avg= (double)e*2/nb;
return (double)avg;
}


// in this method we calculate the number of edges of the densest subgraph and the 
//maximum possible number of edge dans that subgraph
double edge_density2(unsigned long *order, unsigned long nb,adjlist *g){

  double edgeDens ;
  unsigned long e=0;
  unsigned long arret_nb = (double)(nb*(nb -1))/2  ; //the possible edges
  //find edges number
   for(int i=0;i<nb;i++){
     printf("%u\n",i);
     for(int k=0;k<g->e;k++){
        if(g->edges[k].s == order[i] && exists_in_array2(g->edges[k].t, order ,nb)){
        e++;
        } 
     }
   }
  edgeDens = (double)e / arret_nb;
  printf(">>>>edge density = %f\n" , (double)edgeDens);
  return edgeDens;
}

double edge_density3(unsigned long *order, unsigned long nb,adjlist *g){

  double edgeDens ;
  printf("nb%u\n",nb);
  unsigned long e=0;
  unsigned long arret_nb = (double)(nb*(nb -1))/2  ; //the possible edges
  //find edges number
   for(int i=0;i<nb;i++){
     printf("%u\n",i);
     for(int k=0;k<g->e;k++){
        if(g->edges[k].s == order[i] && exists_in_array(g->edges[k].t, order)){
        e++;
        } 
     }
   }
  edgeDens = (double)e / arret_nb;
  printf(">>>>edge density EXO3 = %f\n" , (double)edgeDens);
  return edgeDens;
}


//////////////////////////////EXO 3////////////////////////////////
double *densest_subgraph(adjlist *g, unsigned long t){
  unsigned long u,v;
  double max_density_score=0.0;
  double *r= calloc(g->n,sizeof(double));
    for(int i=0;i<t;i++){
        for(int j=0;j<g->e;j++){
           u=g->edges[j].s;
		       v=g->edges[j].t;
            if(r[u]<=r[v]){
                r[u]++;
            }else{
                r[v]++;
            }
        }
    }
    for(int i=0;i<g->n;i++){
        r[i] = (double)(r[i]/t);
    }
    // in this loop we tried to find max density score that was demanded in exo3
    for(int i=0;i<g->n;i++){
      max_density_score = fmax(r[i],max_density_score);
    }
    printf("max density score of this graph = %f", max_density_score);
    return r;
}

//in this method we searched in first n subgraphs and finding the subgraph with max average_degree_density
//and we returned the number of nodes of that subgraph
 unsigned long core_ordering_prefix_EXO1(unsigned long *order,adjlist* g){
  unsigned int j=0,f=0;
  unsigned nb_node_max_sub =0;
   double max_average_density=0;
   unsigned long nb=0;
  unsigned long *array = calloc(g->n,sizeof(unsigned long));
  for(unsigned i=0;i<1005;i++){
   array[i] = order[i];
    nb++;
    //find max average degree between n first subgraphs
      double m=average_degree_density(array,i+1,g); 
        if(m>max_average_density){
          max_average_density =m;
          nb_node_max_sub=nb;
        }
  }
  printf("max average density = %f , nb de node=%u\n", max_average_density,nb_node_max_sub);
  return nb_node_max_sub;
 }

//like the core_ordering_prefix of first question , here we the same principe but with the new array
 unsigned long core_ordering_prefix_EXO3(unsigned long *nodeScore,adjlist* g){
 
  unsigned int j=0,f=0;
  unsigned long nb_node_max_sub =0;
  double max_average_density=0;
  unsigned long nb=0;
  unsigned long *array = calloc(g->n,sizeof(unsigned long));
   for(int i=0;i<1005;i++){
      array[i] = nodeScore[i];
      nb++;
      //finding the subgraph with maximum average_degree_density
     double m=average_degree_density(array,i+1,g);
        if(m>max_average_density){
          max_average_density =m;
          nb_node_max_sub=nb;
        }
     }
    printf ("\n max degree density = %f ,nb max prefix =  %u\n",max_average_density,nb_node_max_sub);
    return nb_node_max_sub;
 }

//this method is for finding the average degree density of whole graph
  double average_degree_density_total(adjlist *g){
   return (double)g->e*2/g->n;
 }
void swap2Long(unsigned long *a, unsigned long *b){
  unsigned long tmp;
  tmp =*a;
  *a=*b;
  *b=tmp;
}
void swap2Double(double *a, double *b){
  unsigned long tmp;
  tmp =*a;
  *a=*b;
  *b=tmp;
}
// this method is for sorting the array of nodeScore based on their score
void *sort_node_score(adjlist *g, double *score,unsigned long *nodeScore, unsigned long f){
 unsigned long temp,tmp2;
 unsigned long k,m=0;
 if(g->n<f){
   m= g->n;
 }
 if(g->n >f){
   m=f;
 }
 for(int i=0;i<m;i++){
   nodeScore[i]=i;
 }
    for(int i=0;i<m -1;i++){
       for(int j=0;j<m-i-1;j++){
          if(score[j]<score[j+1]){
            swap2Long(&nodeScore[j],&nodeScore[j+1]);
            swap2Double(&score[j],&score[j+1]);
      }
    }
  }
}

//in this method we find the degree of nods who are in the order array and their core for exo2
void find_degree_exo2(unsigned long *order,unsigned long *core,adjlist *g){
  
   unsigned long u;
    printf("I'm in\n");
    char *cout1x = ("ex2x.txt");
    FILE *file1x = fopen(cout1x, "w");

    char *cout1y = ("ex2y.txt");
    FILE *file1y = fopen(cout1y, "w"); 


  for(int i=1; i<g->n-1; i++){
  
    u=i+1;
    fprintf(file1x, "%u\n", core[i]);
    if(i==0){
        fprintf(file1y,"%u\n", g->cd[order[0]]);
    }
    else{
      fprintf(file1y,"%u\n", g->cd[order[u]]-g->cd[order[u]-1]);
    }
    }// end of for
    fclose(file1x);
    fclose(file1y);
 }

int main(int argc, char **argv)
{
  srand(time(NULL));
  adjlist* g;
  time_t t1,t2;
  t1=time(NULL);
  g=readedgelist(argv[1]);
  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);
  mkadjlist(g);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //                                  exo1

  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Binary_heap *tas = construct_min_heap(g);
  unsigned long *core = calloc(g->n, sizeof(unsigned long));
  unsigned long *order = core_decomposition(g,core,tas);
 unsigned long nb_premier= core_ordering_prefix_EXO1(order,g);
 edge_density2(order, nb_premier,g);
 find_degree_exo2(order,core,g); //to finding core value and degree


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //      exo3

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
printf("\n+++++++++++++++++exo3+++++++++++++\n");
unsigned long *nodeScore= calloc(g->n,sizeof(unsigned long));
double *score=densest_subgraph(g, NB_ITER);
//double *score=densest_subgraph(g, NB_ITER_100);
//double *score=densest_subgraph(g, NB_ITER_1000);
sort_node_score(g, score,nodeScore,20000); //sort graph based on their score
unsigned long nb_nodes=core_ordering_prefix_EXO3(nodeScore,g);
printf("nombre de nods %u\n",nb_nodes);
edge_density3(nodeScore,nb_nodes,g);

average_degree_density_total(g);

  free_adjlist(g);
  free(order);
  free(core);
  free(score);
  free(nodeScore);
  t2=time(NULL);
  
  printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  return 0; 
}
