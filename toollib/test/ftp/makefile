LIB_DIR=-lsxftp -L../lib
src=$(wildcard *.cpp)
CC=g++ -g
DEST=main

all:clean dest

dest:
	$(CC) -o$(DEST) $(src) $(LIB_DIR)

clean:
	rm -f *.o $(DEST)


