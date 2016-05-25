src=$(wildcard *.cpp)
obj=$(patsubst %.cpp,%.o,$(src))
dll_LIB=-llog
CC=g++ -g
dest=main
dll_PATH=../log

all:$(dest)

%.o:%.cpp
	$(CC) -c $< $(PIC)

$(dest):$(obj)
	$(CC) -o $@ $^ $(dll_LIB) -L$(dll_PATH)

clean:
	rm -f *.o $(dest)


