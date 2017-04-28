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
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "ServerThread.h"
#include "Auth.h"
#include "CSProtocol.h"
#include "log.h"
// mutex lock for multithreaded access to file transfer
pthread_mutex_t lock_x;
//thread function call
void StartThread(int cs, pthread_mutex_t lock)
{
	//create the thread
	pthread_t pt;
	int* arg = malloc(sizeof(*arg));
	if(arg == NULL)
	{
		fprintf(stderr, "Couldn't Allocate memeory for thread argument.\n");
		exit(EXIT_FAILURE);
	}
	else {
		//Init the thread lock and the socket
		*arg = cs;
		lock_x = lock;
	}
	int thread_return;
	thread_return = pthread_create(&pt, NULL, thread_server, arg);
	if(thread_return)
	{
		fprintf(stderr, "Error - pthread_create() return code: %d\n", thread_return);
		exit(EXIT_FAILURE);
	}
}

// this is where the server thread is created
void *thread_server(void *csock_ptr)
{
	int cs;
	cs = *((int *) csock_ptr);
	HandleClient(cs);
	free(csock_ptr);
}

//this is the main line of execution for the thread
void HandleClient(int cs)
{
	//setup buffer to handles incoming/outgoing messages
	char message[1000];
	int READSIZE;
	int auth = 0;
	char * clientName;
	
	
	//handle authentication initially after client connects before comms begin
	pid_t tid = syscall(__NR_gettid);
	printf("Authenticating User %d\n", tid);
	
	char *username = HandleAuth(cs);
	char *password;
	if(strcmp(username, "") == 0)
	{
		auth = 0;
		printf("Invalid username\\password\n");
	}
	else
	{
		strtok_r(username, " ", &password);
		auth = Authenticate(username, password);
	}

	
	printf("Authentication Result: %d (TRUE=1/FALSE=0)\n", auth);
	//tell the client the auth result
	Authenticated(auth, cs);
	
	//if auth is 0, end session with user
	while(auth)
	{
		printf("User: %s Logged In", username);
		//log the user login
		Log(username, "Logged In");
		// lock the file transfer when a server thread accesses it
		pthread_mutex_lock(&lock_x);
		char *filepath = HandleFileTransfer(cs);
		//unlock after shared data is freed
		pthread_mutex_unlock(&lock_x);

		//if the user wants to end comms
		if(strcmp(filepath, "Quit") == 0)
		{
			close(cs);
			break;
		}
		if(filepath != NULL)
		{
			//log the file transfer
			printf("User: %s\nFile: %s\nTimestamp: %s\n", username, filepath, GetDate());
			LogEntry(username, filepath, GetDate());
			free(filepath);
		}
		else 
		{
			Log(username, "Failed to transfer files");
		}
	}
	if(auth == 0)
	{
		Log(username, "User Failed to Authenticate");
	}
}


