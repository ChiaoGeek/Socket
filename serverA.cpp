#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#define BUFF_SIZE 102400

#define SERVER_PORT 21014
#define SERVER_IP "0.0.0.0"


using namespace std;

void udpServer() {
    // create a socket
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(udpSocket < 0) {
        cerr << "Can not create a udp server";
        return;
    }

    //bind the socket to a IP / port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &hint.sin_addr);

    //bind the socket to a IP / port
    if(::bind(udpSocket, (sockaddr *)&hint, sizeof(hint)) < 0)
    {
        cerr << "Can't bind to IP/port";
        return;
    }

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char buf[BUFF_SIZE];     /* receive buffer */
    /* now loop, receiving data and printing what we received */
    for (;;) {
        int recvlen = recvfrom(udpSocket, buf, BUFF_SIZE, 0, (struct sockaddr *)&client, &clientSize);
        if (recvlen > 0) {
            cout << "received: " << string(buf, 0, recvlen) << endl;
            sendto(udpSocket, buf, strlen(buf), 0, (struct sockaddr *)&client, clientSize);
        }
    }
    /* never exits */

}

int main(int argc, char** argv) {
    cout << "The udp server is up and running" << endl;
    udpServer();
}