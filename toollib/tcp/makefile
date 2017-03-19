src=$(wildcard *.cpp)
obj=$(patsubst %.cpp,%.o,$(src))
PIC=-fPIC
dll_LIB=-shared -lpthread
CC=g++ -g
dest=libyctcp.so

all:$(dest)

%.o:%.cpp
	$(CC) -c $< $(PIC)

$(dest):$(obj)
	$(CC) -o $@ $^ $(dll_LIB) $(PIC)

clean:
	rm -f *.o *.so


