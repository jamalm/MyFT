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
#include <sys/types.h>
#include <sys/syscall.h>
#include "ServerThread.h"
#include "Auth.h"
#include "CSProtocol.h"


//thread function call
void StartThread(int cs)
{
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
}

void *thread_server(void *csock_ptr)
{
	int cs;
	cs = *((int *) csock_ptr);
	HandleClient(cs);
	free(csock_ptr);
	printf("HANDLING FINISHED");
}

void HandleClient(int cs)
{
	//setup buffer to handles incoming/outgoing messages
	char message[1000];
	int READSIZE;
	int auth = 0;
	
	
	//handle authentication initially after client connects before comms begin
	pid_t tid = syscall(__NR_gettid);
	printf("Authenticating User %d\n", tid);
	
	char *username = HandleAuth(cs);
	char *password;
	strtok_r(username, " ", &password);
	auth = Authenticate(username, password);
	
	printf("Authentication Result: %d (TRUE=1/FALSE=0)\n", auth);
	Authenticated(auth, cs);
	
	//if auth is 0, end session with user
	while(auth)
	{
		
		HandleFileTransfer(cs);
		//reset message buffer
		/*memset(message, 0, 1000);
		//read in message
		READSIZE = recv(cs, message, 2000, 0);
		printf("\nMessage received: %s", message);
		
		//check if client disconnected
		if(READSIZE == 0) 
		{
			puts("Client Disconnected!\n");
			fflush(stdout);
			break;
		} 
		else if(READSIZE == -1) 
		{
			perror("\nread error");
			exit(EXIT_FAILURE);
		} 
		else 
		{
			//handle message
			HandleFileTransfer(message, cs);
		}*/
	}
}


