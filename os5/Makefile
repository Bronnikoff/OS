all: client server

client:
	gcc -std=c99 -pthread -Wall client.c -o client -L/usr/local/lib -lzmq

server:
	gcc -std=c99 -pthread -Wall bank.c server.c -o server -L/usr/local/lib -lzmq

clean:
	rm -f *.o client server
