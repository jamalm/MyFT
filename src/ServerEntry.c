/*******************************************************************************************************
Author: 	Jamal Mahmoud 
Date: 		10/04/2017
Desc:		This is where the client connects with the server, the server creates a thread for the client
			and begins authentication before allowing file transfer.
********************************************************************************************************/
#include <stdio.h>		//for IO
#include <sys/socket.h>	//for socket
#include <arpa/inet.h>	//for inet_addr
#include <stdlib.h>		//for exit()
#include <pthread.h>	//for threading
#include "ServerThread.h"

int main(int argc, char *argv[])
{
	int sd;				//socket descriptor
	int clientsock;		//clients socket
	int connSize;		//Size of struct

	
	struct sockaddr_in server, client;
	char message[500];
	
	//create socket
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd == -1)
	{
		printf("Could not create Socket!\nExiting..\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Socket Created Successfully!\n");
	}
	
	//set sockaddr_in vars
	server.sin_port = htons(11109);			//set the port to listen on
	server.sin_family = AF_INET;			//Use IPv4 protocol
	server.sin_addr.s_addr = INADDR_ANY;	//the socket will bind to all local interfaces
	
	//Bind
	if(bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Bind issue!!");
		exit(EXIT_FAILURE);
	} else {
		printf("Bind Complete!!\n");
	}
	
	//start listening and creating threads for each client
	while(1)
	{
		//Listen for a connection
		listen(sd, 3);
		printf("Waiting for incoming connection..\n");
		connSize = sizeof(struct sockaddr_in);
	
		//Accept an incoming Connection
		clientsock = accept(sd, (struct sockaddr *)&client, (socklen_t*) &connSize);
		if(clientsock < 0)
		{
			perror("Can't Establish Connection!\n");
			exit(EXIT_FAILURE);
		} else {
			printf("Connection Established!\n");
			//now initiate thread for client
			StartThread(clientsock);
		}
	}

	
	return 0;
}
