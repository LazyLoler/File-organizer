#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>


void logger(const char *logMessage) 
{
    FILE *log = NULL;
    // Correct usage of fopen_s: pass pointer to FILE*, filename, mode
    if (fopen_s(&log, "events.log", "a+") == 0 && log != NULL)
    {
        char message[1024]; 
        snprintf(message, sizeof(message), "%s\n", logMessage);
        fputs(message, log);
        fclose(log);
    }
}

#endif