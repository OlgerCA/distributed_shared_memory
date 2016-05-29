//
// Created by andres on 28/05/16.
//

#include "FileReader.h"

ClientList* readFile(char* fileName) {
    FILE *file;
    char content[1024], *ip, *port, *line;
    char ch;
    int lines = 0, currentLine = 0;
    ClientList *list = (ClientList*) malloc(sizeof(ClientList));
    ClientInfo **clients;
    file = fopen(fileName, "r");

    if (file == NULL) {
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
        ClientInfo* info = (ClientInfo*) malloc(sizeof(ClientInfo));
        info->clientIP = malloc(strlen(ip) + 1);
        info->clientPort = malloc(strlen(port) + 1);
        strcpy(info->clientIP, ip);
        strcpy(info->clientPort, port);
        clients[currentLine] = info;
        currentLine++;
    }
    fclose(file);
    return list;
}
