CC=gcc
CFLAGS=-g -O9 -lm
EXEC=bfs diameter findTriangle generateNode jaccard kcore

all: $(EXEC)

bfs : bfs.c adjarray.c fifo.c
	$(CC) $(CFLAGS) -o bfs bfs.c adjarray.c fifo.c

diameter : diameter.c adjarray.c fifo.c
	$(CC) $(CFLAGS) -o diameter diameter.c adjarray.c fifo.c

findTriangle : findTriangle.c adjarray.c fifo.c
	$(CC) $(CFLAGS) -o findTriangle findTriangle.c adjarray.c fifo.c

generateNode : GenerateNode.c adjmatrix.c
	$(CC) $(CFLAGS) -o generateNode GenerateNode.c adjmatrix.c 

jaccard : jaccard.c adjarray.c
	$(CC) $(CFLAGS) -o jaccard jaccard.c adjarray.c

kcore : kcore.c adjarray.c
	$(CC) $(CFLAGS) -o kcore kcore.c adjarray.c -lm

clean :
	rm -rf *.o
	rm -rf $(EXEC)
