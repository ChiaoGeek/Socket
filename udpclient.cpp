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

#define SERVER_PORT     21014
#define MAXLINE 1024

// Driver code

void udpClient(string s) {
    int sockfd;
    char buffer[MAXLINE];
    //char *hello = "Hello from client";
    struct sockaddr_in     servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    socklen_t serverSize = sizeof(servaddr);

    int n;


    sendto(sockfd, s.c_str(), s.size(), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
//    n = ::recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &serverSize);

//    cout << "server> " << string(buffer, n) << "\r\n";

//    close(sockfd);
}

int main(int argc, char** argv) {
    cout << "The udp client is up and running" << endl;
    string arguments = "";
    if(argc == 6 || argc == 5) {
        for(int i = 1; i < argc; i++) {
            arguments += argv[i];
            if(i != argc - 1) {
                arguments += " ";
            }
        }
        udpClient(arguments);
    }else {
        cout << "Arguments error" << endl;
    }
}