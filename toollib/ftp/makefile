LIB_DIR=./lib
src=$(wildcard *.cpp)
obj=$(patsubst %.cpp,%.o,$(src))
PIC=-fPIC
LIBSO=-shared
#CC=g++ -gdwarf-2 -gstrict-dwarf
CC=g++ -g

%.o:%.cpp
	$(CC) -c $< $(PIC)

en_decrypt:$(obj)
	$(CC) -o liben_decrypt.so $^ $(LIBSO)
	cp -f liben_decrypt.so ../test/lib

ftp:$(obj)
	$(CC) -o libsxftp.so $^ $(LIBSO)
	cp -f libsxftp.so ../test/lib

log:$(obj)
	$(CC) -o libsinglelog.so $^ $(LIBSO)
	cp -f libsinglelog.so ../test/lib

network:$(obj)
	$(CC) -o libnetwork.so $^ $(LIBSO)
	cp -f libnetwork.so ../test/lib

readini:$(obj)
	$(CC) -o libreadini.so $^ $(LIBSO)
	cp -f libreadini.so ../test/lib

clean:
	rm -f *.o *.so


