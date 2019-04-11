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

#define SERVER_PORT     23014

void udpClient(string s) {

    // Creating socket file descriptor

    int udpClientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if ( udpClientSocket < 0 ) {
        cerr << "Can not create socket" << endl;
        return;
    }

    sockaddr_in  servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    socklen_t serverSize = sizeof(servaddr);

    sendto(udpClientSocket, s.c_str(), s.size(), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

}

int main(int argc, char** argv) {
    cout << "The udp client is up and running" << endl;
    string arguments = "";
    if(argc == 6 || argc == 3 || argc == 8) {
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