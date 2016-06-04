#include "NetworkUtils.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool startsWith(const char *pre, const char *str);

char* getLocalIpAddress(){
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    char *ipAddress = NULL;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if((startsWith("wlan",ifa->ifa_name) == true)&&(ifa->ifa_addr->sa_family==AF_INET))
        {
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            ipAddress = (char *) malloc(strlen(host) * sizeof(char) + 1);
            strcpy(ipAddress, host);
        }
    }

    freeifaddrs(ifaddr);
    if(ipAddress) {
        return ipAddress;
    } else {
        perror("Failed to get local ip address");
        exit(EXIT_FAILURE);
    }
}

bool startsWith(const char *pre, const char *str) {
    size_t lenpre = strlen(pre),
            lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}