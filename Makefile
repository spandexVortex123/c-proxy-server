all:
	gcc server.c utils.c -g -o server -Wall -Wextra
	./server

clean:
	rm server
