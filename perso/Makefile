CC=gcc
CFLAGS=-O9
EXEC=bfs diameter findTriangle generateNode

all: $(EXEC)

bfs : bfs.c adjarray.c fifo.c
	$(CC) $(CFLAGS) -o bfs bfs.c adjarray.c fifo.c

diameter : diameter.c adjarray.c fifo.c
	$(CC) $(CFLAGS) -o diameter diameter.c adjarray.c fifo.c

findTriangle : findTriangle.c adjarray.c fifo.c
	$(CC) $(CFLAGS) -o findTriangle findTriangle.c adjarray.c fifo.c

generateNode : GenerateNode.c adjmatrix.c
	$(CC) $(CFLAGS) -o generateNode GenerateNode.c adjmatrix.c

clean :
	rm -rf *.o
	rm -rf $(EXEC)
