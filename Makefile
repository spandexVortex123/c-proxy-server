all:
	gcc server.c utils.c -g -o server -Wall -Wextra

clean:
	rm server
