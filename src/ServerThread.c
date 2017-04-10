/*******************************************************************************************************
Author: 	Jamal Mahmoud 
Date: 		10/04/2017
Desc:		This file creates the thread and handles the client a client individually using 
			predefined protocols.
********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "ServerThread.h"
//#include "CSProtocol.h"


//thread function call
void StartThread(int cs)
{
	printf("Hello still here");
	pthread_t pt;
	int* arg = malloc(sizeof(*arg));
	if(arg == NULL)
	{
		fprintf(stderr, "Couldn't Allocate memeory for thread argument.\n");
		exit(EXIT_FAILURE);
	}
	else {
		*arg = cs;
	}
	int thread_return;
	thread_return = pthread_create(&pt, NULL, thread_server, arg);
	if(thread_return)
	{
		fprintf(stderr, "Error - pthread_create() return code: %d\n", thread_return);
		exit(EXIT_FAILURE);
	}
	printf("CREATING THREAD");
	pthread_join(pt, NULL);
	printf("THREAD CREATED");
}

void *thread_server(void *csock_ptr)
{
	int cs;
	cs = *((int *) csock_ptr);
	HandleClient(cs);
	free(csock_ptr);
}

void HandleClient(int cs)
{

	char message[500];
	char *response;
	int READSIZE;
	
	
	while(1)
	{
		//reset message buffer
		memset(message, 0, 500);
		//read in message
		READSIZE = recv(cs, message, 2000, 0);
		
		//check if client disconnected
		if(READSIZE == 0) 
		{
			puts("Client Disconnected!\n");
			fflush(stdout);
			exit(EXIT_SUCCESS);
		} 
		else if(READSIZE == -1) 
		{
			perror("read error");
			exit(EXIT_FAILURE);
		} 
		else 
		{
			//handle message
			response = HandleRequest(message);
			//return output message to client
			write(cs, response, strlen(response));
		}
	}
}

char *HandleRequest(char *message)
{
	return "testing all the stuff";
}
