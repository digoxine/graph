#ifndef FIFO_H
#define FIFO_H
#include "bfs.h"

typedef struct {
  unsigned long debut;
  unsigned long fin;
  unsigned long *fifo;
} Fifo;

Fifo* construct_Fifo(adjlist *g);
void add_fifo(Fifo *fifo, unsigned long elem);
unsigned long pop(Fifo* fifo);
int isEmpty(Fifo* fifo);
void free_fifo(Fifo* fifo);
#endif
