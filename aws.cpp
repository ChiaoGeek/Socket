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
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening == -1)
    {
        cerr << "Can't creat a socket";
        return -1;
    }

    //bind the socket to a IP / port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(21563);

    // This function converts the character string src into a network
    // address structure in the af address family, then copies the network
    // address structure to dst.  The af argument must be either AF_INET or
    // AF_INET6.  dst is written in network byte order.
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    // convert sockaddr_in into sockaddr
    // addrlen specifies the size, in bytes, of the
    // address structure pointed to by addr.  Traditionally, this operation
    // is called “assigning a name to a socket”.
    if(bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1)
    {
        cerr << "Can't bind to IP/port";
        return -2;
    }

    //make a socket for listening
    if (listen(listening, SOMAXCONN) == -1)  //max # 128
    {
        cerr << "Can't listen!";
        return -3;
    }
    //accept a call

    for (;;) {

        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        char host[NI_MAXHOST];
        char svc[NI_MAXSERV];

        int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);

        if (clientSocket != -1) // clientSocket is a new socket
        {
            close(listening);
            memset(host, 0, NI_MAXHOST);  //clean up the zombie
            memset(svc, 0, NI_MAXSERV);
            // The getnameinfo() function is the inverse of getaddrinfo(3):
            // it converts a socket address to a corresponding host and service,
            // in a protocol-independent manner. It combines the functionality of gethostbyaddr(3)
            // and getservbyport(3), but unlike those functions, getnameinfo() is reentrant
            // and allows programs to eliminate IPv4-versus-IPv6 dependencies.
            int result = getnameinfo((sockaddr *)&client,
                                     sizeof(client), host, NI_MAXHOST,
                                     svc, NI_MAXSERV, 0);
            if (result)
            {   // ****suppose the svc is the port of client
                cout << host << "connected on " << svc << endl;
            }
            else
            {
                inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
                cout << host <<" connected on " << ntohs(client.sin_port) << endl;
            }
            //while receiving display message, echo message
            char buf[4096];

            memset(buf, 0, 4096);
            //wait for a message;
            while(true) {
                int bytesRecv = recv(clientSocket, buf, 4096, 0);
                if (bytesRecv > 0) {
                    cout << "received: " << string(buf, 0, bytesRecv) << endl;
                    //resent the message;
                    send(clientSocket, buf, bytesRecv + 1, 0);
                }
//                else {
//                    close(clientSocket);
//                    break;
//                }
            }

        }

    }

    //close socket
//    close(clientSocket);

}
