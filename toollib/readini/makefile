src=$(wildcard *.cpp)
obj=$(patsubst %.cpp,%.o,$(src))
PIC=-fPIC
dll_LIB=-shared -lpthread
CC=g++ -g
dest=libreadini.so

all:$(dest)

%.o:%.cpp
	$(CC) -c $< $(PIC)

$(dest):$(obj)
	$(CC) -o $@ $^ $(dll_LIB) $(PIC)
	cp -f $(dest) ../testdll

clean:
	rm -f *.o *.so


