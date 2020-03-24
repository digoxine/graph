#include <stdio.h>
#include <stdlib.h>

double classification_score(int *labels,int nb_clusters, int nb_nodes)
{
  int *visited = malloc(nb_nodes*sizeof(int));
  for(int i=0;i<nb_nodes;i++)
    visited[i]=-1;
  int nb_nodes_by_cluster = nb_nodes/nb_clusters;
  double cpt = 0;
  for(int i=0; i<nb_clusters; i++)
    {
 
      for(int j=0; j<nb_nodes_by_cluster;j++)
	{
	  int real_node = i*nb_nodes_by_cluster + j;
	  if(visited[labels[real_node]]==-1)
	    visited[labels[real_node]] = i;

	  if(visited[labels[real_node]]==i)
	    {
	      cpt++;
	    }
	}
    }  
  return (double) (cpt/ ((double) (nb_nodes)));
}

int *read_labels(char *input, int nb_nodes)
{

  FILE *file = fopen(input,"r");
  int *labels = malloc(sizeof(int)*nb_nodes);
  int k=0;
  int g=0;
  while(fscanf(file,"%u %u",&g,&labels[k])!=EOF)
    {
      k++;
    }
  fclose(file);
  return labels;
}

int get_nb_nodes(char *input)
{
  int g=0;
  int nb_nodes=0;
  FILE *file = fopen(input,"r");
  while(fscanf(file,"%u %u",&g,&g)!=EOF)
    {
      nb_nodes++;
    }
  fclose(file);
  return nb_nodes;
}
int main(int argc, char **argv)
{
  if(argc<3)
    {      
      printf("this program expects the file you want to know the communities clustering and the number of clusters\n");
      return 1;
    }

  int nb_clusters=atoi(argv[1]);
  int nb_nodes = get_nb_nodes(argv[2]);
  int *labels =read_labels(argv[2],nb_nodes);
  double score = classification_score(labels,nb_clusters,nb_nodes);
  printf("%lf\n",score);
  return 0;
}
