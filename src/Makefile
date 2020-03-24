CC=gcc
CFLAGS=-g -O9 
EXEC=bfs diameter findTriangle generateNode jaccard kcore pageRank classification_score label_propagation louvain

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

classification_score: classification_score.c
	$(CC) $(CFLAGS) -o classification_score classification_score.c

kcore : kcore.c adjarray.c
	$(CC) $(CFLAGS) -o kcore kcore.c adjarray.c -lm

pageRank : pageRank.c adjarray.c
	$(CC) $(CFLAGS) -o pageRank pageRank.c adjarray.c

label_propagation : label_propagation.c adjmatrix.c
	$(CC) $(CFLAGS) -o label_propagation label_propagation.c adjmatrix.c

louvain : louvain.c adjarray.c
	$(CC) $(CFLAGS) -o louvain louvain.c adjarray.c

clean :
	rm -rf *.o
	rm -rf $(EXEC)
