#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <time.h>

#define MAX_CLIENTS 30

char* itoa(int number, char string[]) {
    char digits[10] = "0123456789";
    int j = 0;
    int i = number;
    while (i > 0) {
        int mode = i % 10;
        i = i / 10;
        string[j] = digits[mode];
        j++;
    }
    string[j] = '\0';
	if(number == 0) {
		string[0] = '0';
		string[1] = '\0';
	}
	int l = strlen(string);
	for(int i = 0; i < l; i++) {
		int j = l - i - 1;
		char temp;
		if (i < j) {
			temp = string[i];
			string[i] = string[j];
			string[j] = temp;
		}
	}
    return string;
}

int add_client(int server_fd, struct sockaddr_in address, int addrlen) {
	char *accepting_msm = "Request pendding wait for other palyers to join\n"; 
	char buffer[1024] = {0};
	int valread;

	int new_socket;
	if ( (new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
		write(1, "Accepting error!\n", sizeof("Accepting error!\n"));
		return 0;
	}
	write(1, "New request recieved waiting for response from new client\n", sizeof("New request recieved waiting for response from new client\n"));
	return new_socket;
}

int add_to_game(int players[], int size, int player) {
	int flag = 0;
	char num_of_palyers[3];
	char capacity[3];
	itoa(size, capacity);
	for(int i = 0; i < size;  i++)
		if (players[i] == 0) {
			players[i] = player;

			itoa(i+1, num_of_palyers);
			write(1, num_of_palyers, strlen(num_of_palyers));
			write(1, " of ", sizeof(" of "));
			write(1, capacity, strlen(capacity));
			write(1, " players in this game!\n", sizeof("players in this game!\n"));
			
			if (i == (size - 1))
				flag = 1;
			break;
		} 
	return flag;
}

void creat_game(int active_clients[], int clients_size, int group[], int group_size, int port_number) {
	for(int i = 0; i < clients_size; i++)
		for (int j = 0; j < group_size; j++)
			if(active_clients[i] == group[j])
				active_clients[i] = 0;

	char port[10];
	itoa(port_number, port);
	write(1, port, strlen(port));
	write(1, " has been given to this game!\n", sizeof(" has been given to this game!\n"));
	for (int j = group_size-1; j >= 0; j--) {
		char turn[10];
		itoa(j, turn);
		write(1, turn, strlen(turn));
		write(1, "th player got the port number!\n", sizeof("th player got the port number!\n"));
		send(group[j], port, strlen(port), 0);
		send(group[j], turn, strlen(turn), 0);
		group[j] = 0;
	}
}

int random_port() {
	srand(time(0));
	return (rand() % 1000) + 9000;
}

int main(int argc, char const *argv[]) 
{ 	
	int port;
	if(argc >= 2)
		port = atoi(argv[1]);
	else {
		write(1, "You have to enter a port number!\n", sizeof("You have to enter a port number!\n"));
		exit(EXIT_FAILURE);
	}
	
	int two_player_game[2] = {0};
	int three_player_game[3] = {0};
	int four_player_game[4] = {0};
	int active_sockets[MAX_CLIENTS] = {0};
	int server_fd; 
	struct sockaddr_in address; 
	int addrlen = sizeof(address);
	int game_port = 9101; 
	int valread;
	int nready;
	fd_set rset;
	int opt1 = 1;
	int opt2 = 1;
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		write(1, "socket failed\n", sizeof("socket failed\n")); 
		exit(EXIT_FAILURE); 
	} else
		write(1, "Socket created!\n", sizeof("Socket created!\n"));
	
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt1, sizeof(opt1));
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt2, sizeof(opt2));
	
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	address.sin_port = htons( port ); 
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		write(1, "bind failed\n", sizeof("bind failed\n")); 
		exit(EXIT_FAILURE); 
	} else
		write(1, "Binded!\n", sizeof("Binded!\n"));

	if (listen(server_fd, 10) < 0) { 
		write(1, "listen failed\n", sizeof("listen failed\n")); 
		exit(EXIT_FAILURE); 
	} else 
		write(1, "listening...\n", sizeof("listening...\n"));

	
	for ( ; ; ) {
		FD_ZERO(&rset);
		FD_SET(0, &rset);
		FD_SET(server_fd, &rset);
		int max_sd = server_fd;
		for(int i = 0; i < MAX_CLIENTS; i++) {
			int sd = active_sockets[i];
			if(sd > 0)
				FD_SET(sd, &rset);
			if(sd > max_sd)
				max_sd = sd;
		}

		nready = select(max_sd + 1, &rset, NULL, NULL, NULL);
		int new_socket;
		if(FD_ISSET(server_fd, &rset)) {
			new_socket = add_client(server_fd, address, addrlen);
			for(int i = 0; i < MAX_CLIENTS; i++)
				if (active_sockets[i] == 0) {
					active_sockets[i] = new_socket;
					
					// char place[3];
					// itoa(i+1, place);
					// write(1, place, strlen(place));
					// write(1, "th place has got occupied!\n", sizeof("th place has got occupied!\n"));

					break;
				}
			continue;
		}
		if (FD_ISSET(0, &rset)) {
			char input[10];
			read(0, input, sizeof(input));
			if (input[0] == 'e' && input[1] == 'x' && input[2] == 'i' && input[3] == 't')
				break;
		}
		int flag2 = 0;
		int flag3 = 0;
		int flag4 = 0;
		for(int i = 0; i < MAX_CLIENTS; i++) {
			if(active_sockets[i] != 0 && FD_ISSET(active_sockets[i], &rset)) {
				char answer[10];
				valread = read(active_sockets[i], &answer, sizeof(answer));
				answer[valread] = '\0';
				int game_size = atoi(answer);
				// write(1, answer, strlen(answer));
				// write(1, "player game requested!\n", sizeof("player game requested!\n"));
				switch (game_size) {
				case 2:
					flag2 = add_to_game(two_player_game, 2, active_sockets[i]);
					break;
				case 3:
					flag3 = add_to_game(three_player_game, 3, active_sockets[i]);
					break;
				case 4:
					flag4 = add_to_game(four_player_game, 4, active_sockets[i]);
					break;
				default:
					write(1, "Wrong input!\n", sizeof("Wrong input!\n"));
					break;
				}
				break;
			}
		}
		if(flag2) {
			creat_game(active_sockets, MAX_CLIENTS, two_player_game, 2, random_port());
			game_port++;
		}
		if(flag3) {
			creat_game(active_sockets, MAX_CLIENTS, three_player_game, 3, random_port());
			game_port++;
		}
		if(flag4) {
			creat_game(active_sockets, MAX_CLIENTS, four_player_game, 4, random_port());
			game_port++;
		}
	}
	close(server_fd);
} 
