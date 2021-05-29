#include <stdio.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include "game.h" 

int main(int argc, char const *argv[]) 
{ 
	int port;
	if(argc >= 2)
		port = atoi(argv[1]);
	else {
		write(1, "You have to enter a port number!\n", sizeof("You have to enter a port number!\n"));
		exit(EXIT_FAILURE);
	}
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char buffer[1024] = {0}; 
	int opt = 1;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		write(1, "Socket failed!\n", sizeof("Socket failed!\n")); 
		exit(EXIT_FAILURE); 
	} else
		write(1, "Socket created!\n", sizeof("Socket created!\n"));

	serv_addr.sin_family = AF_INET; 
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port = htons(port);

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		write(1, "connection faild!\n", sizeof("connection faild!\n")); 
		exit(EXIT_FAILURE); 
	} else
		write(1, "Socket connected!\n", sizeof("Socket connected!\n"));
	
	int number_of_players;
	bzero(&buffer, sizeof(buffer));
	write(1, "Enter how many players you want to be in the game:(2,3,4) ", sizeof("Enter how many players you want to be in the game:(2,3,4) "));
	read(0, &buffer, sizeof(buffer));
	while(!answer_validator(atoi(buffer))) {
		bzero(&buffer, sizeof(buffer));
		write(1, "Enter how many players you want to be in the game:(2,3,4) ", sizeof("Enter how many players you want to be in the game:(2,3,4) "));
		read(0, &buffer, sizeof(buffer));
	}
	number_of_players = atoi(buffer);
	send(sock , buffer , strlen(buffer) , 0 ); 
	write(1, "message sent: ", sizeof("message sent: "));
	write(1, buffer, sizeof(buffer)); 
	write(1, "waiting for response from server!\n", sizeof("waiting for response from server!\n"));

	fd_set rset;
	int UDP_port;
	int my_turn;
	while(1) {
		FD_ZERO(&rset);
		FD_SET(sock, &rset);
		FD_SET(0, &rset);

		int max_fd = max(0, sock) + 1;
		int activity;
		activity = select(max_fd, &rset, NULL, NULL, NULL);

		if(FD_ISSET(0, &rset)) {
			read(0, buffer, sizeof(buffer));
			write(1, "waiting for response from server!\n", sizeof("waiting for response from server!\n"));
			continue;
		}
		else {
			valread = read( sock , buffer, 4);
			write(1, "The port number is: ", sizeof("The port number is: "));
			write(1, buffer, sizeof(buffer));
			UDP_port = atoi(buffer);
			bzero(buffer, sizeof(buffer));
			valread = read( sock , buffer, 1024);
			write(1, "\nMy turn is: ", sizeof("\nMy turn is: ")); 
			write(1, buffer, sizeof(buffer));
			write(1, "\n", sizeof("\n"));
			my_turn = atoi(buffer);
			close(sock);
			break;
		}
		
	}
	initiate_game(UDP_port, number_of_players, my_turn);
	return 0; 
} 
