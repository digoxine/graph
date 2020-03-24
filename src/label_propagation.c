#include "label_propagation.h"
	  

void fisher_yates(unsigned long *array, int length)
{
  for(int i=1; i<length;i++)
    {
      int r = (int) (rand()%i);
      unsigned long tmp = array[r];
      array[r] = array[i];
      array[i] = tmp;
    }
}

int checkLabelPropagationEnds(adjmatrix *g, unsigned long *labels, unsigned long *copy){

  for(unsigned long i=0; i<g->n; i++)
    {
      
      if(labels[copy[i]]!=highest_frequency(g,copy[i],copy,labels))
	return 1;
    }
  return 0;
}

unsigned long* label_propagation(adjmatrix *g)
{
  unsigned long *labels = calloc(g->n,sizeof(unsigned long));
  unsigned long *copy = calloc(g->n, sizeof(unsigned long));

  //Initialisation
  for(unsigned long i=0; i<g->n; i++)
    {
      labels[(int)(i)] = i;
      copy[i] = i;
    }  

  while(checkLabelPropagationEnds(g,labels,copy)!=0)
    {
      fisher_yates(copy,g->n);
      for(unsigned long i=0; i<g->n;i++)
	{
	  unsigned long actual_node = copy[i];	  
	  unsigned long nd = highest_frequency(g,actual_node,copy,labels);
	  labels[actual_node] = nd;
	}      
    }
  free(copy);
  return labels;
}

//Retourne le label le plus fréquent
unsigned long highest_frequency(adjmatrix *g, unsigned long u,unsigned long *shuffled_array, unsigned long *labels)
{
  unsigned long *array = calloc(g->n,sizeof(unsigned long));

  for(unsigned long i=0;i<g->n;i++)
    {     
      if(g->mat[u*g->n+i]==1)
	{
	  array[labels[i]]++ ;
	}
    }
  unsigned long max = 0;
  unsigned long max_node=0;
  int already_matched = 1;
  int counter_same_node_max = 1;//Lorsque l'on tombe sur un noeud max déjà rencontré on incrémente 0 déja rencontré

  for(unsigned long i=0;i<g->n;i++)
    {      
      if(array[labels[i]]==max)
	{
	  if(already_matched==0)
	    {
	      counter_same_node_max++;
	    }
	  else
	    {
	      counter_same_node_max = 2;
	      already_matched = 0;
	    }
	}
      
      if(array[labels[i]]>max)
	{
	  //Réinitialisation des valeurs de comptages
	  already_matched = 1;
	  counter_same_node_max = 1;
	  max =  array[i];
	  max_node = i;
	  
	}      
    }
  
  if(already_matched==0 && counter_same_node_max>1)
    {     
      unsigned long rd = rand()%(counter_same_node_max);
      unsigned long k=0;
      for(unsigned long i=0; i<g->n; i++)
	{	  
	  if(array[i]==max)
	    {
	      if(k==rd)
		{
		  max_node = i;
		  break;
		}
	      k++;
	    }
	}
      
    }
  free(array);
  return max_node;
    
}


void display_label(unsigned long *label, int length, unsigned long *shuffled_array)
{
  printf("Affichage tableau labels \n");
  for(int i=0; i<length; i++)
    {
      printf("%lu, ",label[i]);
    }
  printf("\n");
}


int main(int argc, char **argv)
{
  srand(time(NULL));
  adjmatrix* g;
  if(argc<2)
    {
      printf("Error. This program needs the file you want to know the communitiesr\n");
      return 1;
    }
  g = readedgelist(argv[1]);
  mkmatrix(g);
  
  unsigned long *labels = label_propagation(g);
  for(int i=0; i<g->n;i++)
    printf("%d %lu\n",i,labels[i]);
  free_adjmatrix(g);

  return 0;
}
