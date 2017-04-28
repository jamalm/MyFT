#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include "log.h"

//standard log
void Log(char *name, char *message)
{
	openlog(name, LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, message);
	closelog();
}
//log for file transfer
void LogEntry(char *username, char *filename, char *timestamp)
{
	openlog(username, LOG_PID|LOG_CONS, LOG_USER);
	char message[500];
	strcpy(message, filename);
	strcat(message, " ");
	strcat(message, timestamp);
	syslog(LOG_INFO, message);
	closelog();
}
// log for errors occurred
void LogErr(char *name, char *message)
{
	openlog(name, LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_ERR, message);
	closelog();
}

// get the timestamp for the log
char* GetDate()
{
	char* date = malloc(sizeof(char)*100);
	//create time stuff
    time_t now;
    struct tm* timeinfo;
    
    //get current time
    time(&now);
    timeinfo = localtime(&now);

	//format string to be our time format, not american
    strftime(date, sizeof(date)*2, "%e-%m-%G", timeinfo);
    
    return date;
}

