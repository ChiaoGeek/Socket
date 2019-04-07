#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main()
{
    //creat a socket
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(udpSocket == -1)
    {
        cerr << "Can't creat a udp socket";
        return -1;
    }

    //bind the socket to a IP / port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(22563);

    // This function converts the character string src into a network
    // address structure in the af address family, then copies the network
    // address structure to dst.  The af argument must be either AF_INET or
    // AF_INET6.  dst is written in network byte order.
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    // convert sockaddr_in into sockaddr
    // addrlen specifies the size, in bytes, of the
    // address structure pointed to by addr.  Traditionally, this operation
    // is called “assigning a name to a socket”.
    if(::bind(udpSocket, (sockaddr *)&hint, sizeof(hint)) == -1)
    {
        cerr << "Can't bind to IP/port";
        return -2;
    }

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char buf[4096];     /* receive buffer */
    /* now loop, receiving data and printing what we received */
    for (;;) {
        int recvlen = recvfrom(udpSocket, buf, 4096, 0, (struct sockaddr *)&client, &clientSize);
        if (recvlen > 0) {
            cout << "received: " << string(buf, 0, recvlen) << endl;
            sendto(udpSocket, buf, strlen(buf), 0, (struct sockaddr *)&client, clientSize);
        }
    }
    /* never exits */

}
