#include "fifo.h"
#include "bfs.h"

Fifo* construct_Fifo(adjlist *g)
{
  Fifo* fifo = malloc(sizeof(Fifo));
  fifo->debut = 0;
  fifo->fin = 0;
  fifo->fifo = malloc(g->n * sizeof(unsigned long));
  return fifo;
}

void add_fifo(Fifo* fifo, unsigned long elem)
{
  fifo->fifo[fifo->fin++] = elem;
}

unsigned long pop(Fifo* fifo)
{
  unsigned long elem = fifo->fifo[fifo->debut];
  fifo->debut = ++fifo->debut;
  return  elem;
}


//return 0 si Fifo vide
int isEmpty(Fifo* fifo)
{
  if(fifo->debut == fifo->fin)
    {
      return 0;
    }
  return 1;
}
