CC=gcc
CFLAGS=-g -O9 
EXEC=bfs diameter findTriangle generateNode jaccard kcore pageRank

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

jaccard_2 : jaccard_2.c adjarray.c
	$(CC) $(CFLAGS) -o jaccard_2 jaccard_2.c adjarray.c -lm

kcore : kcore.c adjarray.c
	$(CC) $(CFLAGS) -o kcore kcore.c adjarray.c -lm

pageRank : pageRank.c adjarray.c
	$(CC) $(CFLAGS) -o pageRank pageRank.c adjarray.c



clean :
	rm -rf *.o
	rm -rf $(EXEC)
