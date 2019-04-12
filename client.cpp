#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>

#include <vector>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 24014

using namespace std;


vector<string> stringToVector(string s) {
    stringstream ss(s);
    string b;
    vector<string> v;
    while(ss >> b) {
        v.push_back(b);
    }
    return v;
}


void startClient(string arg)
{
    //creat a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        return;
    }

    //creat a hint structure for the server we re connecting with

    string ipAddress = SERVER_IP;
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //connect to the server on the socket
    int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if(connectRes == -1)
    {
        return;
    }

    //while loop
    char buf[4096];
    cout << "The client is up and running" << endl;

    //send to server
    vector<string> v = stringToVector(arg);
    int sendRes = send(sock, arg.c_str(), arg.size() + 1, 0);
    string firstWord = v[0];
    if(firstWord.compare("write") == 0) {
        cout << "The client sent write operation to AWS" << endl;
    }else if(firstWord.compare("compute") == 0) {
        cout << "The client sent ID="+v[1]+", size="+v[2]+", and power="+v[3]+" to AWS" << endl;
    }

    if(sendRes == -1)
    {
        cout << "could not send to server!";
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
        vector<string> v_2 = stringToVector(string(buf, bytesReceived));
        string fW = v_2[0];

        if(fW.compare("res") == 0) {
            cout << "The delay for link "+v[1]+" is "+v_2[3]+"ms" << endl;
        }else {
            if(fW.compare("Link") == 0) {
                cout << "Link ID not found" << endl;
            }else {
                cout << "The write operation has been completed successfully" << endl;
            }

        }

    }

    //close the socket

    close(sock);
}

int main(int argc, char** argv) {
    cout << "The client is up and running" << endl;
    string arguments = "";
    if(argc == 6 || argc == 3 || argc == 5 || argc == 8) {
        for(int i = 1; i < argc; i++) {
            arguments += argv[i];
            if(i != argc - 1) {
                arguments += " ";
            }
        }
        startClient(arguments);
//        cout << arguments << endl;
    }else {
        cout << "Arguments error" << endl;
    }
}