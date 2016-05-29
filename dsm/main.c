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


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

char *buffer;
long firstAddress;
int pagesize = 4096;
int numPages = 4;

static void handler(int sig, siginfo_t *si, void *unused)
{
    long faultAddress = (long) si->si_addr;
    long faultingPage = (faultAddress - firstAddress) / pagesize ; //0-based

    if(faultingPage > -1 && faultingPage < numPages) {
        if (mprotect(firstAddress + pagesize * faultingPage, pagesize, PROT_READ) == -1)
            handle_error("mprotect");
    }else{
        //not a segmentation fault we should handle
        handle_error("sigsev");
    }
}

int main(int argc, char *argv[])
{
//    char ch;
//    //default values for arguments
//    char *hostFile = "./hosts";
//    char *logFile = "./logFile";
//    int processCopies = 1;
//    char *processName;
//    char *otherArguments;
//
//    //magic happens here
//    while((ch = (char)getopt(argc, argv, "H:l:n:")) != EOF){
//        switch(ch) {
//            case 'H':
//                hostFile = optarg;
//                break;
//            case 'l':
//                logFile = optarg;
//                break;
//            case 'n':
//                processCopies = (int) strtol(optarg, (char **)NULL, 10);
//                break;
//            default:
//                fprintf(stderr,"Unknown option: '%s'\n", optarg);
//                exit(1);
//        }
//    }
//
//    int index;
//    int totalLength = 0;
//
//    //get the process name
//    processName = malloc(strlen(argv[optind]) * sizeof(char));
//    strcpy(processName, argv[optind]);
//    optind++;
//
//    //get the size of other args to use a single char* to manage them
//    for (index = optind; index < argc; index++){
//        totalLength += strlen(argv[index]) + 1;
//    }
//
//    //actually copy the first extra argument
//    otherArguments = malloc(totalLength * sizeof(char));
//    strcpy(otherArguments, argv[optind]);
//    optind++;
//
//    //concat the other arguments
//    for (index = optind; index < argc; index++){
//        strcat(otherArguments, " ");
//        strcat(otherArguments, argv[index]);
//    }
//
//    //print the arguments :)
//    printf("Arguments: %s, %s, %d\n", hostFile, logFile, processCopies);
//    printf("Executable: %s\n", processName);
//    printf("Non-option arguments: %s\n", otherArguments);
//
//    //read the host file
//    ClientList* clients = readFile(hostFile);
//    int i = 0;
//    for(i; i < clients->size; i++){
//        printf("%s:%s \n", clients->clients[i]->clientIP, clients->clients[i]->clientPort);
//    }

    char *p; char a;
    int* anotherPointer;// = (int*) (malloc(sizeof(int) * 5));

    struct sigaction sa;
    int i = 0;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        handle_error("sigaction");



    /* Allocate a buffer aligned on a page boundary;
       initial protection is PROT_READ | PROT_WRITE */

    buffer = memalign(pagesize, numPages * pagesize);
    if (buffer == NULL)
        handle_error("memalign");

    /*printf("Start of 1st page:        0x%lx\n", (long) buffer);
    printf("Start of 2nd page:        0x%lx\n", (long) buffer+pagesize);
    printf("Start of 3rd page:        0x%lx\n", (long) buffer+2*pagesize);
    printf("Start of 4th page:        0x%lx\n", (long) buffer+3*pagesize);*/

    printf("Start of 1st page:        %lu\n", (long) buffer);
    printf("Start of 2nd page:        %lu\n", (long) buffer+pagesize);
    printf("Start of 3rd page:        %lu\n", (long) buffer+2*pagesize);
    printf("Start of 4th page:        %lu\n", (long) buffer+3*pagesize);

    //protecting the memory from [buffer, buffer + pagesize], i.e, the first page
    firstAddress = buffer;
    if (mprotect(buffer + pagesize * 0, pagesize, PROT_NONE ) == -1)
        handle_error("mprotect");

    //protects the third
    if (mprotect(buffer + pagesize * 2, pagesize, PROT_NONE ) == -1)
        handle_error("mprotect");

    // this will cause the first read to be catched by the handler, the second one does nothing because the memory is not protected
    // then the third causes a fault
    while(i < numPages){
        p = buffer+(pagesize*i++); // moving from page to page
        a = *p; //trying to read the memory
    }
    i = 0;

    //segmentation fault that we shuld not handle
    anotherPointer[45] = 45;

    // will fail for now...
    while(i < numPages){
        p = buffer+(pagesize*i++);
        *(p) = 'a'; //trying to write the memory
    }

    free(p);
    printf("Loop completed\n");
    exit(EXIT_SUCCESS);
}