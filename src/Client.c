/*******************************************************************************************************
Author: 	Jamal Mahmoud 
Date: 		10/04/2017
Desc:		This File creates the Client interface for a user to communicate with the server
********************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int SID;
	struct sockaddr_in server;
	char outBuffer[500];
	char inBuffer[500];
	
	//Create the Socket
	SID = socket(AF_INET, SOCK_STREAM, 0);
	if(SID == -1)
	{
		printf("Error Creating Socket!\nExiting!\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Socket Created\n");
	}
	
	//set socket vars
	server.sin_port = htons(11109);	//port to connect
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); //server IP
	server.sin_family = AF_INET; 	//IPv4 protocol
	
	printf("Connecting...\n");
	//Connect to the server now 
	if(connect(SID, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Connection failed!\n");
		exit(EXIT_FAILURE);
	} else 
	{
		printf("Connected to the Server!\n");
	}
	
	//communicate with server
	while(1)
	{
		printf("\nCommand>>");
		scanf("%s", outBuffer);
		
		//DEFINE PROTOCOL FOR COMMUNICATION
		//send data
		if(send(SID, outBuffer, strlen(outBuffer), 0) < 0)
		{
			printf("Failed to Send!\nExiting due to undefined error..\n");
			exit(EXIT_FAILURE);
		}
		
		//Receive reply from server
		if(recv(SID, inBuffer, 500, 0) < 0)
		{
			printf("Error receiving Message from server: IO Error\n");
			break;
		}
		
		printf("\nSERVER>> %s\n", inBuffer);
	}
	
	close(SID);
	return 0;
}


