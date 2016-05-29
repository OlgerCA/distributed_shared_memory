#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Logger.h"

FILE *file;

static char *format = "[%s] %s: %s\n";

char* levelToString(Level level);
char* getCurrentDateTime();

void logger_init(char *filename) {
    file = fopen(filename, "w+");
}

void logger_log_message(char *message, Level level) {
    fprintf(file, format, levelToString(level), getCurrentDateTime(), message);
    fflush(file);
    puts(message);
}

char* getCurrentDateTime(){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char *dateTime = asctime(tm);
    size_t correctSize = strlen(dateTime);
    char *fixedDateTime = malloc(correctSize * sizeof(char));
    int i = 0;
    for(; i < correctSize - 1; i++){
        fixedDateTime[i] = dateTime[i];
    }
    fixedDateTime[correctSize - 1] = '\0';
    return fixedDateTime;
}

char* levelToString(Level level){
    switch(level){
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

void logger_close() {
    fclose(file);
}
