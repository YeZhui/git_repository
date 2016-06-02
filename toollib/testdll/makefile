src=$(wildcard *.cpp)
obj=$(patsubst %.cpp,%.o,$(src))
dll_LIB=-llog -len_decrypt
CC=g++ -g
dest=main
dll_PATH=-L../log  -L../en_decrypt

all:$(dest)

%.o:%.cpp
	$(CC) -c $< $(PIC)

$(dest):$(obj)
	$(CC) -o $@ $^ $(dll_LIB) $(dll_PATH)

clean:
	rm -f *.o $(dest)


