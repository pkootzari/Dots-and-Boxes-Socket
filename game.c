#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h> 
#include <signal.h>
#include "game.h"

int ctoi(char c) {
    if (c == '0')
        return 0;
    if (c == '1')
        return 1;
    if (c == '2')
        return 2;
    if (c == '3')
        return 3;
    if (c == '4')
        return 4;
    if (c == '5')
        return 5;
    if (c == '6')
        return 6;
    if (c == '7')
        return 7;
    if (c == '8')
        return 8;
    if (c == '9')
        return 9;
}

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

int answer_validator(int answer) {
    if(answer == 2 || answer == 3 || answer == 4)
        return 1;
    return 0;
}

int max(int x, int y) {
    if (x > y)
        return x;
    else return y;
}

int finish_game(int board[4][4], int number_of_players) {
    int flag = 0;
    for(int i = 0; i < number_of_players; i++)
        for (int j = 0; j < number_of_players; j++)
            if (board[i][j] == -1)
                flag = 1;
    return !flag;       
}

int validat_move(int row, int col, int ver_or_hor, int number_of_players,  int vertical_lines[4][5], int horizental_lines[5][4]) {
    if (row == 9 && col == 9 && ver_or_hor == 9)
        return 1;
    
    if (row < 0 || row > number_of_players)
        return 0;
    if (col < 0 || col > number_of_players )
        return 0;
    if (ver_or_hor != 0 && ver_or_hor != 1)
        return 0;
    if (ver_or_hor == 0) 
        if(row >= number_of_players || vertical_lines[row][col] == 1)
            return 0;
    if (ver_or_hor == 1)
        if( col >= number_of_players || horizental_lines[row][col] == 1)
            return 0;
    return 1;
}

int apply_move(int num_of_players, int turn, int row, int col, int ver_or_hor, int board[4][4], int blocks[4][4], int vertical_lines[4][5], int horizental_lines[5][4], int scores[4]) {
    if (row == 9 && col == 9 && ver_or_hor == 9)
        return 1;
    
    int return_value = 1;

    if(ver_or_hor == 0) {
        vertical_lines[row][col]++;
        if(vertical_lines[row][col] > 1)
            write(1, "Ridi vertical", sizeof("Ridi vertical"));
        
        if(col != num_of_players) {
            blocks[row][col]++;
            if(blocks[row][col] == 4) {
                board[row][col] = turn;
                scores[turn]++;
                return_value = 0;
            }
        }

        if(col != 0) {
            blocks[row][col-1]++;
            if(blocks[row][col-1] == 4) {
                board[row][col-1] = turn;
                scores[turn]++;
                return_value = 0; 
            }
        }
    }

    if(ver_or_hor == 1) {
        horizental_lines[row][col]++;
        if(horizental_lines[row][col] > 1)
            write(1, "Ridi horizental", sizeof("Ridi horizental"));

        if(row != num_of_players) {
            blocks[row][col]++;
            if(blocks[row][col] == 4) {
                board[row][col] = turn;
                scores[turn]++;
                return_value = 0;
            }
        } 

        if(row != 0) {
            blocks[row-1][col]++;
            if(blocks[row-1][col] == 4) {
                board[row-1][col] = turn;
                scores[turn]++;
                return_value = 0;
            }
        }
    }

    return return_value;
}

void draw(int num_of_players, int board[4][4], int blocks[4][4], int vertical_lines[4][5], int horizental_lines[5][4]) {
    for(int i = 0; i < num_of_players; i++) {
        
        for(int j = 0; j < num_of_players; j++) {
            write(1, "*", sizeof("*"));
            if(horizental_lines[i][j] == 1) write(1, "---", sizeof("---"));
            else write(1, "   ", sizeof("   "));
        }
        write(1, "*\n", sizeof("*\n"));
        
        for(int j = 0; j < num_of_players; j++) {
            if(vertical_lines[i][j] == 1) write(1, "|", sizeof("|"));
            else write(1, " ", sizeof(" "));
            write(1, "   ", sizeof("   "));
        }
        if(vertical_lines[i][num_of_players] == 1) write(1, "|\n", sizeof("|\n"));
        else write(1, " \n", sizeof(" \n"));

        for(int j = 0; j < num_of_players; j++) {
            if(vertical_lines[i][j] == 1) write(1, "|", sizeof("|"));
            else write(1, " ", sizeof(" "));
            write(1, " ", sizeof(" "));
            if(board[i][j] == -1)
                write(1, " ", sizeof(" "));
            else {
                char s[2];
                itoa(board[i][j], s);
                write(1, s, strlen(s)); 
            }
            write(1, " ", sizeof(" "));
        }
        if(vertical_lines[i][num_of_players] == 1) write(1, "|\n", sizeof("|\n"));
        else write(1, " \n", sizeof(" \n"));

        for(int j = 0; j < num_of_players; j++) {
            if(vertical_lines[i][j] == 1) write(1, "|", sizeof("|"));
            else write(1, " ", sizeof(" "));
            write(1, "   ", sizeof("   "));
        }
        if(vertical_lines[i][num_of_players] == 1) write(1, "|\n", sizeof("|\n"));
        else write(1, " \n", sizeof(" \n"));
    }
    for(int j = 0; j < num_of_players; j++) {
        write(1, "*", sizeof("*"));
        if(horizental_lines[num_of_players][j] == 1) write(1, "---", sizeof("---"));
        else write(1, "   ", sizeof("   "));
    }
    write(1, "*\n", sizeof("*\n"));
}

int get_move(int socket_fd,  struct sockaddr_in baddr, struct sockaddr_in tempaddr) {
    write(1, "waiting for other players move...\n", sizeof("waiting for other players move...\n"));

    char buffer[10] = {0};
    recvfrom(socket_fd, buffer, 7, 0, (struct sockaddr *)&tempaddr, (socklen_t *)sizeof(tempaddr));
    write(1, buffer, sizeof(buffer));
    write(1, " recived from other players!\n", sizeof(" recived from other players!\n"));
    int out[3];
    out[0] = ctoi(buffer[1]);
    out[1] = ctoi(buffer[3]);
    out[2] = ctoi(buffer[5]);
    return out[0]*100+out[1]*10+out[2];
}

void sighandler(int signum) {
    write(1, "player ran out ot time\n", sizeof("player ran out ot time\n"));
}

int send_move(int socket_fd,  struct sockaddr_in baddr, struct sockaddr_in tempaddr, int number_of_players,  int vertical_lines[4][5], int horizental_lines[5][4]) {
    siginterrupt(SIGALRM, 1);
    char buffer[10];
    int out[3];
    while(1) {
        write(1, "Enter your move #row#colunm#vertical_or_horizental#: ", sizeof("Enter your move #row#colunm#vertical_or_horizental#: "));
        bzero(&buffer, sizeof(buffer));
        strcpy(buffer, "#9 9 9 ");
        
        signal(SIGALRM, sighandler);
        alarm(20);
        read(0, &buffer, sizeof(buffer));
        alarm(0);
        
        out[0] = ctoi(buffer[1]);
        out[1] = ctoi(buffer[3]);
        out[2] = ctoi(buffer[5]);
        if (validat_move(out[0], out[1], out[2], number_of_players, vertical_lines, horizental_lines))
            break;
    }

    if(sendto(socket_fd, buffer, 7, 0, (struct sockaddr*)&baddr, sizeof(baddr)) < 0) {
        write(1, "move wan't sent!", sizeof("move wan't sent!"));
        exit(EXIT_FAILURE);
    }
    write(1, buffer, 7);
    write(1, " sent to other players!\n", sizeof(" sent to other players!\n"));
    recvfrom(socket_fd, buffer, 7, 0, (struct sockaddr *)&tempaddr, (socklen_t *)sizeof(tempaddr));
    return out[0]*100+out[1]*10+out[2];
}

void run_around(int number_of_players, int my_turn, int board[4][4], int blocks[4][4], int vertical_lines[4][5], int horizental_lines[5][4], int scores[4],
                int socket_fd, struct sockaddr_in baddr, struct sockaddr_in tempaddr) {
    
    int turn = 0;
    while(turn < number_of_players) {
        if(finish_game(board, number_of_players))
            break;
        int move;
        if(my_turn != turn)
            move = get_move(socket_fd, baddr, tempaddr);
        else 
            move = send_move(socket_fd, baddr, tempaddr, number_of_players, vertical_lines, horizental_lines);

        int row, col, ver_or_hor;
        ver_or_hor = move%10;
        move = move / 10;
        col = move % 10;
        move = move / 10;
        row = move;
        
        int new_turn = apply_move(number_of_players, turn, row, col, ver_or_hor,  board, blocks, vertical_lines, horizental_lines, scores);
        draw(number_of_players, board, blocks, vertical_lines, horizental_lines);
        turn += new_turn;
    }
}

void print_scores(int scores[4], int my_turn, int number_of_players) {
    char output[10];
    for(int i = 0; i < number_of_players; i++) {
        bzero(&output, sizeof(output));
        if(i == my_turn)
            write(1, "me", sizeof("me"));
        else {
            itoa(i, output);
            write(1, output, strlen(output));
        }
        
        write(1, ": ", sizeof(": "));
        bzero(&output, sizeof(output));
        itoa(scores[i], output);
        write(1, output, strlen(output));
        write(1, "\n", sizeof("\n"));
    }
}

void initiate_game(int port_nember, int number_of_players, int my_turn) {
    int socket_fd;
    int opt1 = 1;
    int bdcst = 1;
    int opt2 = 1;
    struct sockaddr_in baddr, tempaddr;
    
    if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        write(1, "Socket failed!", sizeof("Socket failed!"));
        exit(EXIT_FAILURE);
    } else
        write(1, "Broadcasting socket created!\n", sizeof("Broadcasting socket created!\n"));
    
    setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &bdcst, sizeof(bdcst));
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &opt1, sizeof(opt1));
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt2, sizeof(opt2));
    
    bzero(&baddr, sizeof(baddr));
    baddr.sin_family = AF_INET;
    baddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    baddr.sin_port = htons(port_nember);
    bzero(&tempaddr, sizeof(tempaddr));
    tempaddr.sin_family = AF_INET;
    tempaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    tempaddr.sin_port = htons(port_nember);

    if(bind(socket_fd, (struct sockaddr *)&baddr, sizeof(baddr)) < 0) {
        write(1, "binding failed!", sizeof("binding failed!"));
        exit(EXIT_FAILURE);
    } else
        write(1, "socket binded!\n", sizeof("socket binded!\n"));


    int scores[4] = {0};
    int board[4][4]; // who had a cell
    for(int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            board[i][j] = -1;
    int blocks[4][4] = {0}; // how many lines has the cell
    int vertical_lines[4][5] = {0};
    int horizental_lines[5][4] = {0};
    int turn = 0;
    
    while(!finish_game(board, number_of_players)) {
        // write(1, "new round begins\n", sizeof("new round begins\n"));
        run_around(number_of_players, my_turn, board, blocks, vertical_lines, horizental_lines, scores, socket_fd, baddr, tempaddr);
        
    }
    print_scores(scores, my_turn, number_of_players);
    close(socket_fd);
    
}
