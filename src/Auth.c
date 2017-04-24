#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Auth.h"
#define USERS "./users/auth"
#define MAXUSERS 256
#define MAXUNAMESIZE 1000


int Authenticate(char *user, char *password)
{
	int i, j;
	printf("Entered Auth\n");
	char storedUser [500];
	char* storedPass;
	//read file for stored usernames
	char** auths = getusers();
	
	printf("User1: %s\nUser2: %s\nUSer3: %s\n", auths[0], auths[1], auths[2]);
	for(i=0;i<MAXUSERS;i++)
	{
		if(auths[i] != "")
		{
			printf("\n---------------------------\nAuth combi string: %s\n", auths[i]);
			//split by username and password
			strtok_r(auths[i], " ", &storedPass);
			strncpy(storedUser, auths[i], sizeof(storedUser));
			storedPass[strcspn(storedPass, "\n")] = 0;
			
			printf("Auth Password: %s\nAuth Username: %s\n-----------------------------\n", storedPass, storedUser);

			if((strcmp(user, storedUser) == 0) && (strcmp(password, storedPass) == 0))
			{
				printf("Comparison return: TRUE\n");
				return 1;
			}
		}
	}
	printf("Comparison return: FALSE\n");
	return 0;
}

char ** getusers()
{
	printf("LOADING USERS FILE...\n");
	int i;
	int counter = 0;
	char buffer[MAXUNAMESIZE];
	
	
	//allocate a size for the number of users allowed to log into the system
	char** auths = malloc(MAXUSERS * sizeof(char *));
	
	for(i=0; i<MAXUSERS; i++)
	{
		auths[i] = malloc(MAXUNAMESIZE+1);
	}
	
	FILE* fp = fopen(USERS, "r");
	if(fp == NULL)
	{
		perror("unable to open auth file");
	}
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		strncpy(auths[counter], buffer, sizeof(buffer));
		fprintf(stdout, "Added: %s from buffer with string %s\nLOCATION: %d", auths[counter], buffer, counter);
		counter++;
	}
	fclose(fp);
	return auths;
}
