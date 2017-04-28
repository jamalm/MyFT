#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "CSProtocol.h"
#include "log.h"
#define MAX_BUF 500
#define MAXFILESIZE 4096


//server protocols
//notification for client on authentication
void Authenticated(int auth_flag, int sockfd)
{
	if(auth_flag)
	{
		send(sockfd, "pass", strlen("pass"), 0);
	}
	else 
	{
		send(sockfd, "fail", strlen("fail"), 0);
	}
}
//handles the transfer of the file
char *HandleFileTransfer(int sockfd)
{

	int bytes = 0;	//for the number of bytes transferred
	char *filelocation = (char *)malloc(MAX_BUF * sizeof(char *));	//this is the location of the file that is returned after file IO is complete
	//string literals for the directories
	const char *root = "/var/html/";
	const char *sales = "/var/html/Sales/";
	const char *promos = "/var/html/Promotions/";
	const char *offers = "/var/html/Offers/";
	const char *market = "/var/html/Marketing/";
	const char *dirs[] = {root, sales, promos, offers, market};
	const char *DIR = "/var/html";

	//various buffers for information received from the client
	char loc[MAX_BUF];
	char file_name[MAX_BUF];
	char dest_folder[MAX_BUF];
	char file_size[MAX_BUF];
	char file_contents[MAXFILESIZE];
	
	//receive file name
	if(recv(sockfd, file_name, MAX_BUF, 0) < 0)
	{
		LogErr("File Transfer", "Failed to receive filename");
		perror("file transfer error");
	}
	if(strcmp(file_name, "") == 0)
	{
		printf("Error receiving data\n");
		return NULL;
	}
	else if(strcmp(file_name, "Quit") == 0)
	{
		return "Quit";
	}
	//printf("received filename: %s\n", file_name);
	//respond confirmation
	send(sockfd, file_name, MAX_BUF, 0);
	printf("\nFile name: %s\n", file_name);
	
	
	if(recv(sockfd, dest_folder, MAX_BUF, 0) < 0)
	{
		LogErr("File Transfer", "Failed to receive destination folder");
		perror("file transfer error");
	}
	printf("Destination folder: %s\n", dest_folder);
	//respond confirmation
	send(sockfd, dest_folder, MAX_BUF, 0);
	
	

	printf("Reading in File Contents..\n");
	//receive file
	
	if(recv(sockfd, file_contents, sizeof(file_contents), 0) < 0)
	{
		LogErr("File Transfer", "Failed to receive File Data");
		perror("File Transfer error");
	}
	printf("Contents:\n%s\n", file_contents);
	
	//write file to disk
	// 1. Make sure the directory requested is valid
	// 2. Pass that directory in and open it
	// 3. Write to it and close it
	
	
	//creating the directory requested
	strcpy(loc, DIR);
	strncat(loc, dest_folder, sizeof(dest_folder));
	int i;	//counter
	for(i=0;i<5;i++)
	{
		if(strcmp(loc, dirs[i]) == 0)
		{	
			//if the directory requested
			strcpy(filelocation, dirs[i]);
			strcat(filelocation, file_name);
			
			//open the file
			FILE *fp;
			fp = fopen(filelocation, "w");
			//if it fails to open
			if(fp == NULL)
			{
				LogErr("File Transfer", "Failed to open file");
				perror("unable to open file");
			}
			Log("Opened file for Writing", filelocation);
			printf("opened file for writing\n");
			bytes = fwrite(file_contents, 1, strlen(file_contents), fp);
			if(bytes < 0)
			{
				LogErr("File Transfer", "Failed to Write to file");
				perror("Failed to write to file");
			}
			else {
				Log("File Transfer", "Wrote to file");
				printf("Wrote to file!\n");
			}
			fclose(fp);
			break;
		}
	}

	//respond with the number of bytes written to file
	char response[MAX_BUF] = { };
	if(bytes > 0)
	{
		snprintf(response, MAX_BUF-1 ,"Successfully transferred %d bytes to server", bytes);
	} else 
	{
		filelocation = "[INVALID]";
		snprintf(response, MAX_BUF-1 ,"Failed to Transfer, invalid folder");
	}
	
	printf("Response: %s", response);
	//send result of file transfer
	send(sockfd, response, MAX_BUF, 0);
	
	return filelocation;
}
char *HandleAuth(int sockfd)
{
	//handles the input of authentication of username and password
	char *inBuffer = (char *)malloc(MAX_BUF);
	recv(sockfd, inBuffer, MAX_BUF, 0);
	return inBuffer;
}

//client protocols
//sends credentials to server
int Auth(char *username, char *password, int sockfd)
{
	char inBuffer[MAX_BUF];
	char outBuffer[MAX_BUF];
	memset(inBuffer, 0, 500);
	memset(outBuffer, 0, 500);
	
	strcpy(outBuffer, username);
	strcat(outBuffer, " ");
	strcat(outBuffer, password);
	//client sends username
	send(sockfd, outBuffer, strlen(outBuffer), 0);
	//handle response
	recv(sockfd, inBuffer, sizeof(inBuffer), 0);
	//printf("\nSERVER RESP: %s", inBuffer);
	if(strcmp(inBuffer, "pass") == 0)
	{
		return 1;
	}
	else if(strcmp(inBuffer, "fail") == 0)
	{
		return 0;
	}
}
//sends the file information and contents
char *FileTransfer(char *file_path, char *fileContents, char *dest_file, int sockfd)
{
	char *inBuffer = (char *)malloc(MAXFILESIZE);
	//client sends src folder
	send(sockfd, file_path, MAX_BUF, 0);
	//printf("Sent: %s\n", file_path);
	recv(sockfd, inBuffer, MAX_BUF, 0);
	//printf("Receiving filepath confirmed: %s\n", inBuffer);
	memset(inBuffer, 0, MAX_BUF);
	//send dest folder
	send(sockfd, dest_file, MAX_BUF, 0);
	//printf("Sent: %s\n", dest_file);
	recv(sockfd, inBuffer, MAX_BUF, 0);

	send(sockfd, fileContents, MAXFILESIZE, 0);		
	//reset the buffer
	memset(inBuffer, 0, MAXFILESIZE);
	//wait for response
	recv(sockfd, inBuffer, MAXFILESIZE, 0);

	return inBuffer;
}

//universal
void EndConnection(int sockfd)
{
	char *quit = "Quit";
	send(sockfd, quit, sizeof(quit)-1, 0);
}


