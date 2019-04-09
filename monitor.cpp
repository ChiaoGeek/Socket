#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fstream>
#include <ostream>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 25014

using namespace std;



void startClient()
{
    //creat a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        return;
    }

    //creat a hint structure for the server we re connecting with

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(SERVER_PORT);
    string serverIP = SERVER_IP;
    inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

    //connect to the server on the socket
    int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if(connectRes == -1)
    {
        return;
    }
    string arg = "send";
    //while loop
    char buf[4096];
    do {
//        cout << "stop1" << endl;
        int sendRes = send(sock, arg.c_str(), arg.size() + 1, 0);
//        cout << "stop2" << endl;
        if(sendRes == -1)
        {
            cout << "could not send to server!";
        }

        //wait for response
        memset(buf, 0, 4096);
//        cout << "stop3" << endl;
        int bytesReceived = recv(sock, buf, 4096, 0);
//        cout << "stop4" << endl;
        if (bytesReceived == -1)
        {
            cout << "there was an error getting from the server\r\n";
        }
        else
        {
            //display response
            string res = string(buf, bytesReceived);
            if(res.compare("empty") != 0) {
                cout << "server> " << res << "\r\n";
            }
        }
        sleep(1);
    }while(true);
    close(sock);
}

int main(int argc, char** argv) {
    cout << "The monitor client is up and running" << endl;
    startClient();
}
