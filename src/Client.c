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
#include "CSProtocol.h"
char *GetFileContents(char *);

int main(int argc, char *argv[])
{
	int SID;
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
		printf("AUTH: %d", auth);
		
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
		
		printf("-----------------------------------------------\nSend File to server (FORMAT: >>NameOfFile RemotePath)\nAvailable dest directories:\n/\n/Sales\n/Promotions\n/Offers\n/Marketing\n-----------------------------------------------\n>>");
		fgets(localfile, sizeof(localfile), stdin);
		printf("\nREAD IN %s\n", localfile);
		
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
		//get contents
		printf("Path to File is: %s\n", fullPath);
		file = GetFileContents(fullPath);
		printf("Got file contents\n");
		
		
		//DEFINE PROTOCOL FOR COMMUNICATION
		//send data
		inBuffer = FileTransfer(localfile, file, remotepath, SID);
		/*
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
		}*/
		
		printf("\nSERVER>> %s\n", inBuffer);
	}
	
	close(SID);
	return 0;
}

char *GetFileContents(char *filename)
{
	printf("Entered Function\n");
	char *buffer = NULL;
	size_t size = 0;
	struct stat st;
	
	printf("opening file\n");
	FILE *fp = fopen(filename, "r");
	printf("File Pointer: %d\n", fp);
	stat(filename, &st);
	size = st.st_size;
	
	printf("Size of File: %d\n", size);
	/*
	//get file length
	fseek(fp, 0, SEEK_END);	//go to end of file
	size = ftell(fp);	//how many bytes passed
	
	//set position to zero
	rewind(fp);
	*/
	//allocate sufficient memory to buffer
	buffer = malloc((size+1) * sizeof(*buffer));	//+1 for the \0
	
	//read the fle into the buffer
	fread(buffer, size, 1, fp);//read in file
	buffer[size] = '\0';//NULL-terminate buffer
	
	return buffer;
	
}


