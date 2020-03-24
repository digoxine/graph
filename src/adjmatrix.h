#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed

typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	bool *mat;//adjacency matrix
} adjmatrix;
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c);
adjmatrix* readedgelist(char* input);
void mkmatrix(adjmatrix* g);
void free_adjmatrix(adjmatrix *g);
