/*

#include <stdio.h>

int main (int argc, char *argv[])
{
    puts("Hello from dsm central manager");
    return 0;
}
*/

#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <getopt.h>
#include <stdbool.h>
#include "FileReader.h"
#include "NetworkInfo.h"
#include "Server.h"
#include "ServerHandle.h"
#include "Logger.h"

#define NUMBER_OF_PAGES 4096

#define SERVER_IP_ADDRESS "192.168.0.1"
#define SERVER_PORT "4400"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    char ch;
    //default values for arguments
    char *hostFile = "./hosts";
    char *logFile = "./logFile";
    int processCopies = 1;
    char *processName;
    char *otherArguments = NULL;

    //magic happens here
    while((ch = (char)getopt(argc, argv, "H:l:n:")) != EOF){
        switch(ch) {
            case 'H':
                hostFile = optarg;
                break;
            case 'l':
                logFile = optarg;
                break;
            case 'n':
                processCopies = (int) strtol(optarg, (char **)NULL, 10);
                break;
            default:
                fprintf(stderr,"Unknown option: '%s'\n", optarg);
                exit(1);
        }
    }
    logger_init(logFile);

    int index;
    int totalLength = 0;

    if(argc <= optind){
        printf("Executable file parameter missing\n");
        exit(-1);
    }

    //get the process name
    processName = malloc(strlen(argv[optind]) * sizeof(char));
    strcpy(processName, argv[optind]);
    optind++;

    char logProcess[50];
    sprintf(logProcess, "Executable file: %s", processName);
    logger_log_message(logProcess, INFO);

    //get the size of other args to use a single char* to manage them
    for (index = optind; index < argc; index++){
        totalLength += strlen(argv[index]) + 1;
    }

    //print the arguments :)
    printf("Arguments: %s, %s, %d\n", hostFile, logFile, processCopies);
    printf("Executable: %s\n", processName);

    if(optind < argc){
        //actually copy the first extra argument
        otherArguments = malloc(totalLength * sizeof(char));
        strcpy(otherArguments, argv[optind]);
        optind++;

        //concat the other arguments
        for (index = optind; index < argc; index++){
            strcat(otherArguments, " ");
            strcat(otherArguments, argv[index]);
        }
        printf("Non-option arguments: %s\n", otherArguments);
    }

    //read the host file
    ClientList* clients = readFile(hostFile);

    server_startup(NUMBER_OF_PAGES, processCopies);
    int cx = server_open(PORT, processCopies);
    if (cx == -1) {
        handle_error("socket");
    }

    int i = 0;
    for(; i < processCopies; i++){
        char command[1024];
        int correctIndex = i % clients->size;
        if(otherArguments){
            sprintf(command, "ssh %s@%s '%s %s %s %ld' &",
                    clients->clients[correctIndex]->clientUsername,
                    clients->clients[correctIndex]->clientIP,
                    processName,
                    otherArguments,
                    clients->clients[correctIndex]->clientIP, //Esto no deberia de ser el IP del server?
                    clients->clients[correctIndex]->clientPort); //Esto no deberia de ser el port del server?
        } else {
            sprintf(command, "ssh %s@%s '%s %s %ld' &",
                    clients->clients[correctIndex]->clientUsername,
                    clients->clients[correctIndex]->clientIP,
                    processName,
                    clients->clients[correctIndex]->clientIP, //Esto no deberia de ser el IP del server?
                    clients->clients[correctIndex]->clientPort); //Esto no deberia de ser el port del server?
        }
        logger_log_message(command, INFO);
        clients->clients[correctIndex]->clientPort++;
        //system(command);
    }

    server_catch(cx, processCopies);
    server_teardown();

    exit(EXIT_SUCCESS);
}
