/*******************************************************************************************************
Author: 	Jamal Mahmoud 
Date: 		10/04/2017
Desc:		This File creates the Client interface for a user to communicate with the server
********************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <termcap.h>
#include "CSProtocol.h"
char *GetFileContents(char *);
int exists(char *);
void clear_screen();
int SID = 0;

void endComms()
{
	if(SID != 0)
	{
		printf("ENDING COMMS\n ");
		EndConnection(SID);
		close(SID);
		exit(EXIT_SUCCESS);
	}
	else
	{
		printf("SID: %d", SID);
	}
}


void sig_handler(int sigNum)
{
	if(sigNum == SIGINT)
	{
		//end communications
		endComms();
		printf("\nExiting Program...\n");
		exit(EXIT_SUCCESS);
	}
}


int main(int argc, char *argv[])
{
	
	if(signal(SIGINT, sig_handler) == SIG_ERR)
	{
		printf("Error with SIGINT, unsafe execution, aborting...\n");
		exit(EXIT_FAILURE);
	}
	int auth = 0;
	struct sockaddr_in server;
	char outBuffer[1000];
	char *inBuffer;
	
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
	server.sin_port = htons(atoi(argv[1]));	//port to connect
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
		clear_screen();
		char username[500];
		char password[500];
		
		printf("Connected to the Server!\n");
		printf("\nEnter your username: ");
		//get username and remove newline
		fgets(username, 500, stdin);
		username[strcspn(username, "\n")] = 0;
		
		//same with password
		printf("\nPassword: ");
		fgets(password, 500, stdin);
		password[strcspn(password, "\n")] = 0;
		
		//send them to server
		auth = Auth(username, password, SID);
		
		if(auth == 0)
		{
			printf("Failed to Authenticate!\nExiting...");
		}
	}
	
	
	
	//communicate with server
	while(auth)
	{
		char localfile[500];
		char *file;
		char *remotepath;
		char fullPath[500];

		printf("-----------------------------------------------\nSend File to server (FORMAT: >>NameOfFile /RemotePath/)\nAvailable dest directories:\n/\n/Sales/\n/Promotions/\n/Offers/\n/Marketing/\nEnter \"exit\" to quit!\n-----------------------------------------------\n>>");
		fgets(localfile, sizeof(localfile), stdin);
		if(strcmp(localfile, "exit\n") == 0)
		{
			//exits the application
			endComms();
		}
		
		//split local filename from the remotepath arg
		strtok_r(localfile, " ", &remotepath);
		remotepath[strcspn(remotepath, "\n")] = 0;

		//get current directory
		char filepath[500];
		getcwd(filepath, 100);
		strcat(filepath, "/");
		printf("\nCurrent working Directory is %s\n", filepath);
		strcpy(fullPath, filepath);
		strcat(fullPath, localfile);
		
		if((exists(fullPath)) == 0)
		{
			//get contents
			file = GetFileContents(fullPath);
			inBuffer = FileTransfer(localfile, file, remotepath, SID);
		}
		else 
		{
			inBuffer = "File does not exist";
		}
		clear_screen();
		printf("\nSERVER>> %s\n", inBuffer);
	}
	
	close(SID);
	return 0;
}

char *GetFileContents(char *filename)
{

	char *buffer = NULL;
	size_t size = 0;
	struct stat st;

	FILE *fp = fopen(filename, "r");

	stat(filename, &st);
	size = st.st_size;
	
	printf("Size of File: %d\n", size);

	//allocate sufficient memory to buffer
	buffer = malloc((size+1) * sizeof(*buffer));	//+1 for the \0
	
	//read the fle into the buffer
	fread(buffer, size, 1, fp);//read in file
	buffer[size] = '\0';//NULL-terminate buffer
	
	return buffer;
	
}

int exists(char *filename)
{
	FILE *file;
	if(file = fopen(filename, "r"))
	{
		fclose(file);
		return 0;
	}
	return 1;
}

void clear_screen()
{
	//see www.stackoverflow.com/questions/17271576/clear-screen-in-c-and-c-on-unix-based-system
	char buf[1024];
	char *str;
	tgetent(buf, getenv("TERM"));
	str = tgetstr("cl", NULL);
	fputs(str, stdout);
}

