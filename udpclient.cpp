// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

#include <iostream>

#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <string>

using namespace std;

#define PORT     22563
#define MAXLINE 1024

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    //char *hello = "Hello from client";
    struct sockaddr_in     servaddr;
    string s = "hello";
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;

    //sendto(sockfd, (const char *)hello, strlen(hello),0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    sendto(sockfd, s.c_str(), s.size(),
           0, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
    printf("Hello message sent.\n");

//    n = ::recvfrom(sockfd, (char *)buffer, MAXLINE,
//                 MSG_WAITALL, (struct sockaddr *) &servaddr,
//                 &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}