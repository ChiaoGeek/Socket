#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int startClient()
{
    //creat a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        return 1;
    }

    //creat a hint structure for the server we re connecting with
    int port = 21563;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //connect to the server on the socket
    int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if(connectRes == -1)
    {
        return 1;
    }

    //while loop
    char buf[4096];
    string userInput;

    do{
        //enter lines of text
        cout << ">";
        getline(cin, userInput);
        //send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if(sendRes == -1)
        {
            cout << "could not send to server!";
            continue;
        }

        //wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cout << "there was an error getting from the server\r\n";
        }
        else
        {
            //display response
            cout << "server> " << string(buf, bytesReceived) << "\r\n";
        }

    } while(true);
    //close the socket

    close(sock);
}

int main(int argc, char** argv) {
    count << "The client is up and running" << endl;

}