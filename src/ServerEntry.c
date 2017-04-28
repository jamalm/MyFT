/*******************************************************************************************************
Author: 	Jamal Mahmoud 
Date: 		10/04/2017
Desc:		This is where the client connects with the server, the server creates a thread for the client
			and begins authentication before allowing file transfer.
********************************************************************************************************/
#include <stdio.h>		//for IO
#include <sys/socket.h>	//for socket
#include <sys/types.h>	
#include <sys/stat.h>	//these are for checking and creating folders
#include <unistd.h>
#include <arpa/inet.h>	//for inet_addr
#include <stdlib.h>		//for exit()
#include <pthread.h>	//for threading
#include "ServerThread.h"

void setup_directories();

int main(int argc, char *argv[])
{
	//sets up the directory structure required prior to running the server
	setup_directories();
	int sd;				//socket descriptor
	int clientsock;		//clients socket
	int connSize;		//Size of struct
	pthread_mutex_t lock_x;
	
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
	server.sin_port = htons(atoi(argv[1]));			//set the port to listen on
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
	
	pthread_mutex_init(&lock_x, NULL);
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
			StartThread(clientsock, lock_x);
		}
	}

	
	return 0;
}

// This initialises the directories required to run the server transfer
void setup_directories()
{
	struct stat st = {0};
	int i = 0;	//counter
	const char *root = "/var/html/";
	const char *sales = "/var/html/Sales/";
	const char *promos = "/var/html/Promotions/";
	const char *offers = "/var/html/Offers/";
	const char *market = "/var/html/Marketing/";
	const char *dirs[] = {root, sales, promos, offers, market};
	
	//create directories that are not already there
	for(i=0;i<5;i++)
	{
		if(stat(dirs[i], &st) == -1)
		{
			mkdir(dirs[i], 0700);
		}
	}
}
