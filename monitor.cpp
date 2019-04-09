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

using namespace std;

void writeToFile(string filename, string content) {
    ofstream myfile;
    myfile.open(filename.c_str());
    myfile << content;
    myfile.close();
}

bool isEmpty(string filename){
    ifstream file;
    file.open(filename.c_str());
    bool res = file.peek() == ifstream::traits_type::eof();
    file.close();
    return res;
}

string getLineFromFile(string filename) {
    string res;
    ifstream file;
    file.open(filename.c_str());
    if(isEmpty(filename)) {
        res = "empty";
    }else {
        getline(file, res);
    }
    file.close();
    return res;
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
    int port = 25014;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //connect to the server on the socket
    int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if(connectRes == -1)
    {
        return;
    }

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
            string res = getLineFromFile("test.txt");
            cout << "server> " << res << "\r\n";
            if(!res.compare("empty")) {
                cout << "server> " << res.size() << "\r\n";
            }
        }
        arg = "test";
    }while(true);

    close(sock);
}

int main(int argc, char** argv) {
    cout << "The monitor client is up and running" << endl;
    string arguments = "";
    if(argc == 6 || argc == 5) {
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
