#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "CSProtocol.h"
#define MAX_BUF 500
#define MAXFILESIZE 4096


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
	printf("\nHandling File transfer..\n");
	const char *DIR = "/var/html";
	char loc[MAX_BUF];
	char file_name[MAX_BUF];
	char dest_folder[MAX_BUF];
	char file_size[MAX_BUF];
	char file_contents[MAXFILESIZE];
	
	
	
	//receive file name
	if(recv(sockfd, file_name, MAX_BUF, 0) < 0)
	{
		perror("filename error");
	}
	printf("received filename: %s\n", file_name);
	//respond confirmation
	send(sockfd, file_name, MAX_BUF, 0);
	printf("\nFile name: %s\n", file_name);
	
	
	
	//receive dest folder name
	recv(sockfd, dest_folder, MAX_BUF, 0);
	printf("Destination folder: %s\n", dest_folder);
	//respond confirmation
	send(sockfd, dest_folder, MAX_BUF, 0);
	
	
	/*
	printf("Receiving size of file..\n");
	char size[50] = { };
	//receive size of file
	int numbytes = recv(sockfd, size, sizeof(size), 0);
	if(numbytes < 0)
	{
		perror("Size of file error");
	}
	else {
		printf("Number of bytes read: %d", numbytes);
	}
	printf("char size of file  is: %s\n", size);
	int s = atoi(size);
	printf("Size of file: %d\n", s);
	//respond with size
	send(sockfd, size, sizeof(size), 0);
	
	*/
	
	
	
	printf("Reading in File Contents..\n");
	//receive file
	recv(sockfd, file_contents, sizeof(file_contents), 0);
	printf("Contents:\n%s\n", file_contents);
	//write file to disk
	strcpy(loc, DIR);
	strncat(loc, dest_folder, sizeof(dest_folder));
	strncat(loc, file_name, sizeof(file_name));
	printf("Location to save: %s\n", loc);
	FILE *fp;
	fp = fopen(loc, "w");
	if(fp == NULL)
	{
		perror("unable to open file");
	}
	printf("opened file for writing\n");
	int bytes = fwrite(file_contents, 1, strlen(file_contents), fp);
	if(bytes < 0)
	{
		perror("Failed to write to file");
	}
	else {
		printf("Wrote to file!\n");
	}
	char response[5] = { };
	int check = snprintf(response, 4 ,"%d", bytes);
	
	//send result of file transfer
	send(sockfd, response, MAX_BUF, 0);
	fclose(fp);
	printf("Returning to loop\n");
}
char *HandleAuth(int sockfd)
{
	char *inBuffer = (char *)malloc(MAX_BUF);
	recv(sockfd, inBuffer, MAX_BUF, 0);
	return inBuffer;
}

//client protocols
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
	printf("\nSending file..\n");
	printf("file path is %s with a length of %d\n", file_path, strlen(file_path));
	printf("destination file is %s\n", dest_file);
	printf("Contents of the file is \n%s\nIT HAS A SIZE OF %d\n", fileContents, strlen(fileContents));
	char *inBuffer = (char *)malloc(MAXFILESIZE);
	//client sends src folder
	send(sockfd, file_path, MAX_BUF, 0);
	printf("Sent: %s\n", file_path);
	recv(sockfd, inBuffer, MAX_BUF, 0);
	printf("Receiving filepath confirmed: %s\n", inBuffer);
	memset(inBuffer, 0, MAX_BUF);
	//send dest folder
	send(sockfd, dest_file, MAX_BUF, 0);
	printf("Sent: %s\n", dest_file);
	recv(sockfd, inBuffer, MAX_BUF, 0);
	printf("Receiving dest_file confirmed: %s\n", inBuffer);
	/*
	//send size of file
	char size[MAX_BUF];
	memset(inBuffer, 0, MAX_BUF);
	memset(size, 0, MAX_BUF);
	printf("setting size of file %s which is %d\n", file_path, strlen(fileContents));
	int check = snprintf(size, MAX_BUF - 1, "%d", strlen(fileContents));
	printf("Size: %s\n", size);
	send(sockfd, size, MAX_BUF, 0);
	recv(sockfd, inBuffer, MAX_BUF, 0);
	printf("Receiving size of file confirmed: %s\n", inBuffer);
	int s = atoi(size);
	//if confirmed size, send file contents over 
	if(s == atoi(inBuffer))
	{
		*/
	send(sockfd, fileContents, MAXFILESIZE, 0);		
	printf("Sent: %s\n", fileContents);
	//}
	memset(inBuffer, 0, MAXFILESIZE);
	//wait for response
	recv(sockfd, inBuffer, MAXFILESIZE, 0);
	printf("Receiving Number of bytes read into server: %s\n", inBuffer);
	return inBuffer;
}

//universal
void EndConnection(int sockfd)
{
	send(sockfd, "Quit", strlen("Quit"), 0);
}


