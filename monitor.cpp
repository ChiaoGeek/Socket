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
#include <vector>
#include <sstream>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 25014

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
    cout << "The monitor is up and running." << endl;
    //while loop
    char buf[4096];
    do {
        int sendRes = send(sock, arg.c_str(), arg.size() + 1, 0);
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
            string id = "2";
            string res = string(buf, bytesReceived);
            if(res.compare("empty") != 0) {

                vector<string> v = stringToVector(res);
                string firstWord = v[0];

                if(firstWord.compare("write") == 0) {
                    cout << "The monitor received BW = "+v[1]+", L = "+v[2]+", V = "+v[3]+" and P = "+v[4]+" from the AWS" << endl;
                }else if(firstWord.compare("compute") == 0) {
                    id = v[1];
                    cout << "The monitor received link ID="+v[1]+", size="+v[2]+", and power="+v[3]+" from the AWS" << endl;
                }else if(firstWord.compare("res") == 0) {
                    cout << "The result for link "+id+": Tt = "+v[1]+"ms, Tp = "+v[2]+"ms, Delay = "+v[3]+"ms" << endl;
                }else if(firstWord.compare("id") == 0) {
                    cout << "The write operation has been completed successfully" << endl;
                }else if(firstWord.compare("notFound") == 0) {
                    cout << "Link ID not found" << endl;
                }
            }
        }


    }while(true);
    close(sock);
}

int main(int argc, char** argv) {
    startClient();
}
