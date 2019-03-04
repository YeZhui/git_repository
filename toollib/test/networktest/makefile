LIB_DIR=../lib
CC=g++ -g -W
.PHONY:testclient testserver epollserver

testclient:
	rm -f testclient
	$(CC) -otestclient TestClient.cpp -lpthread -lnetwork -L$(LIB_DIR)

testserver:
	rm -f testserver
	$(CC) -otestserver TestServer.cpp -lpthread -lnetwork -L$(LIB_DIR)

epollserver:
	rm -f epollserver
	$(CC) -oepollserver TestEpollServer.cpp -lpthread -lnetwork -lsinglelog -lreadini -L$(LIB_DIR)

clean:
	rm -f *.o testclient testserver epollserver


