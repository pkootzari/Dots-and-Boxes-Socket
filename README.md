# Dots and Boxes with Sockets

## Description

This is a simple Dots and Boxes game implemented in client-server model. After server starts running an a port clients can establish a TCP connection with server and ask for a room to play. Rooms are 2, 3 and 4 players. After finding enough players clients will use UDP comminucation system to play among themselves.

## How to run

Make the project using the makefile:

```make```

Run the server with port as argument:

```./server <port>```

Run the clients with server port as argument:

```./client <server_port>```

## How to play

Each turn the player enters three numbers to place a line in the following format: #row#column#0 for vertical and 1 for horizental line

As an example:

```#0#1#0```

This means a horizontal line in first row and second column.
