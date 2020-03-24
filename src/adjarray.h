#ifndef ADJARRAY_H
#define ADJARRAY_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define NLINKS 100000000

typedef struct {
  unsigned long nbNoeudsvisites;
  unsigned int taille;
  unsigned long *visited;
}Visited;

typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
} adjlist;

inline unsigned long max3(unsigned long a, unsigned long b, unsigned long c);

#include "fifo.h"

adjlist* readedgelist(char* input);
void mkadjlist(adjlist* g);
void free_adjlist(adjlist *g);
void markNode(Visited *v, unsigned long s);
int FirstNodeNotVisited(Visited *visit);
int allVisited(Visited *visit);
#endif
