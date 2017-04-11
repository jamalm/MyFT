#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Auth.h"

int Authenticate(char *user, char *password)
{
	printf("Entered Auth\n");
	char *storedUser = "Jamal";
	char *storedPass = "1234";
	printf("User: %s|\n", user);
	printf("Stored User: %s|\n", storedUser); 
	printf("PASS: %s|\n", password);
	printf("storedPASS: %s|\n", storedPass);
	if((strcmp(user, storedUser) == 0) && (strcmp(password, storedPass) == 0))
	{
		printf("Comparison return: TRUE\n");
		return 1;
	}
	else 
	{
		printf("Comparison return: FALSE\n");
		return 0;
	}
}
