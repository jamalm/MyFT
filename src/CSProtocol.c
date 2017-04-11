#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "CSProtocol.h"

//server protocols
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
void HandleFileTransfer(int sockfd)
{
	const char *DIR = "/var/html";
	char loc[500];
	char *file_name;
	char *dest_folder;
	char *file_size;
	char *file_contents;
	//receive file name
	recv(sockfd, file_name, sizeof(file_name), 0);
	//respond confirmation
	send(sockfd, file_name, sizeof(file_name), 0);
	printf("\nFile name: %s\n", file_name);
	//receive dest folder name
	recv(sockfd, dest_folder, sizeof(dest_folder), 0);
	//respond confirmation
	send(sockfd, dest_folder, sizeof(dest_folder), 0);
	printf("Destination folder: %s\n", dest_folder);
	//receive size of file
	recv(sockfd, file_size, sizeof(file_size), 0);
	//respond with size
	send(sockfd, file_size, sizeof(file_size), 0);
	printf("Size of file: %s\n", file_size);
	//receive file
	recv(sockfd, file_contents, sizeof(file_contents), 0);
	//write file to disk
	strcpy(loc, DIR);
	strncat(loc, dest_folder, sizeof(dest_folder));
	strncat(loc, file_name, sizeof(file_name));
	FILE *fp = fopen(loc, "w");
	char *size;
	int bytes = write(fp, file_contents, sizeof(file_contents));
	sprintf(size, "%d", bytes);
	
	//send result of file transfer
	send(sockfd, size, sizeof(size), 0);
}
char *HandleAuth(int sockfd)
{
	char *inBuffer = (char *)malloc(sizeof(char)*500);
	recv(sockfd, inBuffer, 500, 0);
	return inBuffer;
}

//client protocols
int Auth(char *username, char *password, int sockfd)
{
	char inBuffer[500];
	memset(inBuffer, 0, 500);
	//client sends username
	send(sockfd, username, strlen(username), 0);
	//client sends password
	send(sockfd, password, strlen(password), 0);
	//handle response
	recv(sockfd, inBuffer, sizeof(inBuffer), 0);
	printf("\nSERVER RESP: %s", inBuffer);
	if(strcmp(inBuffer, "pass") == 0)
	{
		return 1;
	}
	else if(strcmp(inBuffer, "fail") == 0)
	{
		return 0;
	}
}
char *FileTransfer(char *file_path, char *fileContents, char *dest_file, int sockfd)
{
	char *inBuffer = (char *)malloc(sizeof(char)*1000);
	//client sends src folder
	send(sockfd, file_path, strlen(file_path), 0);
	recv(sockfd, inBuffer, 1000, 0);
	//send dest folder
	send(sockfd, dest_file, 1000, 0);
	recv(sockfd, inBuffer, 1000, 0);
	
	//send size of file
	char *size;
	memset(inBuffer, 0, sizeof(inBuffer));
	sprintf(size, "%d", strlen(fileContents));
	send(sockfd, size, sizeof(size), 0);
	recv(sockfd, inBuffer, sizeof(inBuffer), 0);
	
	//if confirmed size, send file contents over 
	if(atoi(inBuffer) == atoi(size))
	{
		send(sockfd, fileContents, sizeof(fileContents), 0);
	}
	memset(inBuffer, 0, sizeof(inBuffer));
	//wait for response
	recv(sockfd, inBuffer, 1000, 0);
	return inBuffer;
}

//universal
void EndConnection(int sockfd)
{
	send(sockfd, "Quit", strlen("Quit"), 0);
}


