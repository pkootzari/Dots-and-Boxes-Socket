all: Server

Server : 
	gcc server.c -o server
	gcc client.c game.c -o client

# game.o: game.c game.h
# 	gcc -c game.c

# client.c: client.c 
# 	gcc -c client.c 

clean:
	rm *.o server client