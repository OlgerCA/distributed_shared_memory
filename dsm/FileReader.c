#include "FileReader.h"
#include "Logger.h"

ClientList* readFile(char* fileName) {
    logger_log_message("Reading client config file", INFO);
    FILE *file;
    char content[1024], *ip, *port, *username, *line;
    char ch;
    int lines = 0, currentLine = 0;
    ClientList *list = (ClientList*) malloc(sizeof(ClientList));
    ClientInfo **clients;
    file = fopen(fileName, "r");

    if (file == NULL) {
        logger_log_message("Error reading file", ERROR);
        printf("Error reading file");
        exit(1);
    }

    while(!feof(file)) { ch = fgetc(file); if(ch == '\n') { lines++; } }
    clients = (ClientInfo **) malloc(sizeof(ClientInfo*) * lines);
    list->clients = clients;
    list->size = lines;
    rewind(file);
    while (fgets(content, 1024, file)) {
        line = strtok(content, "\n");
        ip = strtok(line, ":");
        port = strtok(NULL, ":");
        username = strtok(NULL, ":");
        ClientInfo* info = (ClientInfo*) malloc(sizeof(ClientInfo));
        info->clientIP = malloc(strlen(ip) + 1);
        info->clientUsername = malloc(strlen(username) + 1);
        strcpy(info->clientIP, ip);
        info->clientPort = strtol(port, NULL, 0);
        strcpy(info->clientUsername, username);
        clients[currentLine] = info;
        currentLine++;
    }
    fclose(file);
    return list;
}
