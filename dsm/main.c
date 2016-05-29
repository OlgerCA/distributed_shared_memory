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

#define NUMBER_OF_PAGES 4096

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
    char *otherArguments;

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

    int index;
    int totalLength = 0;

    //get the process name
    processName = malloc(strlen(argv[optind]) * sizeof(char));
    strcpy(processName, argv[optind]);
    optind++;

    //get the size of other args to use a single char* to manage them
    for (index = optind; index < argc; index++){
        totalLength += strlen(argv[index]) + 1;
    }

    //actually copy the first extra argument
    otherArguments = malloc(totalLength * sizeof(char));
    strcpy(otherArguments, argv[optind]);
    optind++;

    //concat the other arguments
    for (index = optind; index < argc; index++){
        strcat(otherArguments, " ");
        strcat(otherArguments, argv[index]);
    }

    //print the arguments :)
    printf("Arguments: %s, %s, %d\n", hostFile, logFile, processCopies);
    printf("Executable: %s\n", processName);
    printf("Non-option arguments: %s\n", otherArguments);

    //read the host file
    ClientList* clients = readFile(hostFile);
    int i = 0;
    for(i; i < clients->size; i++){
        printf("%s:%s \n", clients->clients[i]->clientIP, clients->clients[i]->clientPort);
    }

    server_startup(clients->size, NUMBER_OF_PAGES);
    int cx = server_open(PORT, MAXCONN);
    if (cx == -1) {
        handle_error("socket");
    }
    server_catch(cx, MAXCONN);
    server_teardown();

    printf("Loop completed\n");
    exit(EXIT_SUCCESS);
}